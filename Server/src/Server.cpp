#include "Server.hpp"

Server::Server(void(*handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*))
    : m_listenThread(&Server::listen, this), m_running(false), mysql(this), POSITION_UPDATE_INTERVAL(0.5)
{
    // Bind a packet handler function.
    m_packetHandler = std::bind(handler,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                std::placeholders::_4, std::placeholders::_5);
}

Server::~Server()
{
    stop();
}

void Server::connectMySQL()
{
    std::cout << "Connecting to MySQL server... \n";
    if(!mysql.connect())
    {
        std::cout << "FATAL-ERROR: Failed to connect to database!\nPress r to retry.\n";
        char x = getchar();
        cout << x << endl;
        stop();
    }
    else
        std::cout << "Connected to database.\n";
}

void Server::setup()
{
    m_running = false;
    m_totalSent = 0;
    m_totalReceived = 0;
}

bool Server::start()
{
    if (m_running)
        return false;

    if (m_incoming.bind((unsigned short)Network::ServerPort) != sf::Socket::Done)
        return false;

    //m_outgoing.bind( int(Network::ClientsPort) );
    m_outgoing.bind( sf::Socket::AnyPort );
    setup();
    std::cout << "Server IP: " << sf::IpAddress::getLocalAddress() << ":" << int(Network::ServerPort) << std::endl;
    std::cout << "Outgoing port: " << m_outgoing.getLocalPort() << std::endl;

    m_listenThread.launch();
    m_running = true;
    return true;
}

bool Server::stop()
{
    if (!m_running)
        return false;

    disconnectAll();
    m_running = false;
    m_incoming.unbind(); // Stops the listening thread.
    return true;
}

void Server::disconnectAll()
{
    if (!m_running)
        return;

    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin(); itr != m_clients.end(); )
    {
        removeClient(itr);
        itr++;
    }
}

/*
void Server::bindTimeoutHandler(void(*handler)(const ClientID&))
{
    m_timeoutHandler = std::bind(handler, std::placeholders::_1);
}
*/

bool Server::send(const ClientID& id, sf::Packet& packet)
{
    sf::Lock lock(m_mutex);
    auto itr = m_clients.find(id);
    if (itr == m_clients.end())
        return false;

    if (m_outgoing.send(packet, itr->second.m_clientIP, itr->second.m_clientPORT) != sf::Socket::Done)
    {
        std::cout << "Error sending a packet..." << std::endl;
        return false;
    }
    m_totalSent += packet.getDataSize();
    return true;
}

bool Server::send(sf::IpAddress& ip, const PortNumber& port, sf::Packet& packet)
{
    if (m_outgoing.send(packet, ip, port) != sf::Socket::Done)
    {
        return false;
    }
    m_totalSent += packet.getDataSize();
    return true;
}

void Server::broadcast(sf::Packet& packet, const ClientID& ignore)
{
    sf::Lock lock(m_mutex);
    for (auto &itr : m_clients)
    {
        if (itr.first != ignore)
        {
            if (m_outgoing.send(packet, itr.second.m_clientIP, itr.second.m_clientPORT)!= sf::Socket::Done)
            {
                std::cout << "Error broadcasting a packet to client: " << itr.first << std::endl;
                continue;
            }
            m_totalSent += packet.getDataSize();
        }
    }
}

void Server::listen()
{
    sf::IpAddress ip;
    PortNumber port;
    sf::Packet packet;
    std::cout << "Beginning to listen...\n";
    while (m_running)
    {
        packet.clear();
        sf::Socket::Status status = m_incoming.receive(packet, ip, port);
        if (status != sf::Socket::Done)
        {
            if (m_running)
            {
                std::cout << "Error receiving a packet from: " << ip << ":" << port << ". Code: " << status << std::endl;
                continue;
            }
            else
            {
                std::cout << "Socket unbound." << std::endl;
                break;
            }
        }

        m_totalReceived += packet.getDataSize();

        PacketID p_id;
        if (!(packet >> p_id))
        {
            std::cout << "Invalid packet received: unable to extract id." << std::endl;
            continue;
        } // Non-conventional packet.

        PacketType id = (PacketType)p_id;

        if (id < PacketType::Disconnect || id >= PacketType::OutOfBounds)
        {
            std::cout << "Invalid packet received: id is out of bounds." << std::endl;
            continue;
        } // Invalid packet type.

        if (id == PacketType::Heartbeat)
        {
            bool ClientFound = false;
            sf::Lock lock(m_mutex);
            for (auto &itr : m_clients)
            {
                if (itr.second.m_clientIP != ip || itr.second.m_clientPORT != port)
                    continue;

                ClientFound = true;
                if (!itr.second.m_heartbeatWaiting)
                {
                    std::cout << "Invalid heartbeat packet received!" << std::endl;
                    break;
                }
                //std::cout << "Client " << itr.second.m_clientIP << " ping: " << itr.second.m_ping << std::endl;

                itr.second.m_ping = m_serverTime.asMilliseconds() - itr.second.m_heartbeatSent.asMilliseconds();
                itr.second.m_lastHeartbeat = m_serverTime;
                itr.second.m_heartbeatWaiting = false;
                itr.second.m_heartbeatRetry = 0;
                break;
            }
            if (!ClientFound)
            {
                std::cout << "Heartbeat from unknown client received...\n";
            }
        }
        else if (m_packetHandler)
        {
            m_packetHandler(ip, port, (PacketID)id, packet, this); // Handle the packet.
        }
    }
    std::cout << "...Listening stopped.\n";
}

void Server::update(const sf::Time& time)
{
    m_serverTime += time;
    m_positionUpdateTime += time.asSeconds();
    updatePositions();

    if (m_serverTime.asMilliseconds() < 0)
    {
        m_serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
        sf::Lock lock(m_mutex);

        for (auto &itr : m_clients)
        {
            itr.second.m_lastHeartbeat = sf::milliseconds(std::abs(itr.second.m_lastHeartbeat.asMilliseconds() - sf::Int32(Network::HighestTimestamp)));
        }
    }

    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin(); itr != m_clients.end(); )
    {
        if( itr->second.m_character != nullptr )
            itr->second.m_character->update(time.asSeconds());

        sf::Int32 elapsed = m_serverTime.asMilliseconds() - itr->second.m_lastHeartbeat.asMilliseconds();
        if (elapsed >= HEARTBEAT_INTERVAL)
        {
            if (elapsed >= sf::Int32(Network::ClientTimeout) || itr->second.m_heartbeatRetry > HEARTBEAT_RETRIES)
            {
                /*
                // Remove client.
                if (m_timeoutHandler)
                {
                    m_timeoutHandler(itr->first);
                }
                */
                std::cout << "Client " << itr->first << " has timed out." << std::endl;

                removeClient(itr);
                //itr = m_clients.erase(itr);
                continue;
            }
            if (!itr->second.m_heartbeatWaiting || (elapsed >= HEARTBEAT_INTERVAL * (itr->second.m_heartbeatRetry + 1)))
            {
                // Heartbeat
                /*
                if (itr->second.m_heartbeatRetry >= 3)
                    std::cout << "Re-try(" << itr->second.m_heartbeatRetry << ") heartbeat for client " << itr->first << std::endl;
                */
                sf::Packet Heartbeat;
                stampPacket(PacketType::Heartbeat, Heartbeat);
                Heartbeat << m_serverTime.asMilliseconds();

                send(itr->first, Heartbeat);

                if (itr->second.m_heartbeatRetry == 0)
                    itr->second.m_heartbeatSent = m_serverTime;

                itr->second.m_heartbeatWaiting = true;
                ++itr->second.m_heartbeatRetry;

                m_totalSent += Heartbeat.getDataSize();
            }
        }

        itr++;
    }
}

void Server::updatePositions()
{
    if(m_positionUpdateTime < POSITION_UPDATE_INTERVAL)
        return;

    m_positionUpdateTime -= POSITION_UPDATE_INTERVAL;

    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin(); itr != m_clients.end(); )
    {
        if( itr->second.m_character == nullptr)
        {
            itr++;
            continue;
        }

        sf::Packet packet;
        stampPacket(PacketType::Move, packet);

        for (auto itr2 = m_clients.begin(); itr2 != m_clients.end();)
        {
            if( itr2->second.m_character == nullptr)
            {
                itr2++;
                continue;
            }

            if( itr->second.m_character->getLocation() == itr2->second.m_character->getLocation())
            {
                packet << itr2->second.m_character->getId();
                packet << itr2->second.m_character->getSnapshot();
            }
            itr2++;
        }
        send(itr->second.m_clientIP, itr->second.m_clientPORT, packet);
        itr++;
    }
}

ClientID Server::addClient(const sf::IpAddress& ip, const PortNumber& port, const sf::Uint16 & cid, const std::string & username)
{
    sf::Lock lock(m_mutex);

    for (auto &itr : m_clients)
    {
        if (itr.second.m_clientIP == ip && itr.second.m_clientPORT == port)
            return ClientID(Network::NullID);
    }
    ClientInfo info(ip, port, m_serverTime, username, cid);
    m_clients.emplace(cid, info);

    return cid;
}

ClientID Server::addClient(const ClientInfo & client)
{
    sf::Lock lock(m_mutex);

    for (auto &itr : m_clients)
    {
        if ( (itr.second.m_clientIP == client.m_clientIP
        &&    itr.second.m_clientPORT == client.m_clientPORT )
        ||    itr.second.m_clientID == client.m_clientID)   //already logged in
            return ClientID(Network::NullID);
    }
    m_clients.emplace(client.m_clientID, client);
    return client.m_clientID;
}

void Server::removeClient(auto & itr)
{
    sf::Lock lock(m_mutex);

    sf::Packet packet;
    stampPacket(PacketType::Disconnect, packet);
    send(itr->second.m_clientID, packet);

    //delete pointer to entity
    removeCharacter(itr->second.m_clientID);
    itr = m_clients.erase(itr);
}

bool Server::removeClient(const ClientID& id)
{
    sf::Lock lock(m_mutex);
    auto itr = m_clients.find(id);
    if (itr == m_clients.end())
        return false;

    sf::Packet packet;
    stampPacket(PacketType::Disconnect, packet);
    send(id, packet);

    removeCharacter(id);
    m_clients.erase(itr);
    return true;
}

bool Server::removeClient(const sf::IpAddress& ip, const PortNumber& port)
{
    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin(); itr != m_clients.end(); ++itr)
    {
        if (itr->second.m_clientIP == ip && itr->second.m_clientPORT == port)
        {
            sf::Packet packet;
            stampPacket(PacketType::Disconnect, packet);
            send(itr->first, packet);
            removeCharacter(itr->second.m_clientID);
            m_clients.erase(itr);
            return true;
        }
    }
    return false;
}

bool Server::hasClient(const ClientID& id)
{
    return (m_clients.find(id) != m_clients.end());
}

bool Server::hasClient(const sf::IpAddress& ip, const PortNumber& port)
{
    return(getClientID(ip, port) >= 0);
}

bool Server::isRunning() { return m_running; }

bool Server::getClientInfo(const ClientID& id, ClientInfo& info)
{
    sf::Lock lock(m_mutex);
    for (auto &itr : m_clients)
    {
        if (itr.first == id)
        {
            info = itr.second;
            return true;
        }
    }
    return false;
}

ClientInfo * Server::findClient(const ClientID& id)
{
    for (auto &itr : m_clients)
        if (itr.first == id)
            return &itr.second;

    return nullptr;
}

Entity * Server::findEntity(const unsigned & id)
{
    for (auto &itr : m_clients)
        if (itr.second.m_character->getId() == id)
            return itr.second.m_character;

    return nullptr;
}

ClientID Server::getClientID(const sf::IpAddress& ip, const PortNumber& port)
{
    sf::Lock lock(m_mutex);
    for (auto &itr : m_clients)
        if (itr.second.m_clientIP == ip && itr.second.m_clientPORT == port)
            return itr.first;

    return ClientID(Network::NullID);
}

unsigned int Server::getClientCount()
{
    return m_clients.size();
}

std::string Server::getClientList()
{
    std::string list;
    std::string delimiter = "--------------------------------------";
    list = delimiter;
    list += '\n';
    list += "ID";
    list += '\t';
    list += "Client IP:PORT";
    list += '\t';
    list += '\t';
    list += "Ping";
    list += '\n';
    list += delimiter;
    list += '\n';
    for (auto itr = m_clients.begin(); itr != m_clients.end(); ++itr)
    {
        list += std::to_string(itr->first);
        list += '\t';
        list += itr->second.m_clientIP.toString() + ":" + std::to_string(itr->second.m_clientPORT);
        list += '\t';
        list += std::to_string(itr->second.m_ping) + "ms.";
        list += '\n';
    }
    list += delimiter;
    list += '\n';
    list += "Total data sent: " + std::to_string(m_totalSent / 1000) + "kB. Total data received: " + std::to_string(m_totalReceived / 1000) + "kB";
    return list;
}

sf::Mutex& Server::getMutex()
{
    return m_mutex;
}

sf::Time Server::getServerTime()const
{
    return m_serverTime;
}

void Server::removeCharacter(const ClientID& id)
{
    sf::Lock lock(m_mutex);
    auto itr = m_clients.find(id);
    if (itr == m_clients.end())
        return;

    if( itr->second.m_character != nullptr)
    {
        mysql._SaveCharacter(itr->second.m_character);
        delete itr->second.m_character;
        itr->second.m_character = nullptr;
    }
}

Clients Server::getClients() { return m_clients; }
