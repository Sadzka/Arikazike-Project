#include "Server\Client.hpp"

void Client::handlePacket(const sf::Uint16& id, sf::Packet& packet, Client * client)
{
    switch( static_cast<PacketType>(id) )
    {
    case PacketType::Connect:
        {
            cout << "Zalogowano pomyslnie\n";
            break;
        }
    case PacketType::Message:
        {
            std::string message;
            packet >> message;
            std::cout << message << std::endl;
            break;
        }
    case PacketType::Disconnect:
        {
            cout<<"Dostalem pakiet od servera ze mam DC\n";
            disconnect();
            break;
        }
    case PacketType::GetCharacterList:
        {
            m_shared->m_entityManager->purge();
            int much;
            packet >> much; //how much
            for (int i = 0; i < much; i++ )
            {
                sf::Uint32 cid;
                std::string str;
                packet>>str>>cid;
                m_shared->m_entityManager->add(EntityType::Player, str, cid);
            }
            m_waitPacket = false;
            break;
        }
    case PacketType::Move:
        {
            sf::Uint32 id;
            while (packet >> id)
                m_shared->m_entityManager->updateEntity(packet, id);


            if(m_unknownEntities.size() > 0)
            {
                cout <<"Unknown entities - " << m_unknownEntities.size() << endl;;
                getCharacterDetails();
            }
            break;
        }
    case PacketType::GetCharacterDetails:
        {
            for(;;)
            {
                Entity * entity = new Entity( m_shared->m_entityManager );
                if( packet >> *entity )
                {
                    if(entity == nullptr)
                    {
                        delete entity;
                        continue;
                    }

                    switch( entity->getRace() )
                    {
                    case EntityRace::Human:
                        entity->getSpriteSheet().loadSheet("Data\\Units\\Human.txt");
                        //entity->setTexture( m_shared->m_textureManager->getResource("Data\\img\\Units\\Human.png") );
                        break;
                    default:
                        cout << "[" << int(entity->getRace()) << "] Unknown type\n";
                        break;
                    }
                    entity->move(0, 0);
                    m_shared->m_entityManager->add(entity);
                    if(m_shared->m_entityManager->getSize() == 1)
                    {
                        m_player = entity;
                        m_waitPacket = false;
                    }
                }
                else
                {
                    delete entity;
                    break;
                }
            }
            break;
        }
    default:
        {
            std::cout<<"Unknown packet type " + std::to_string(id) + "\n";
            break;
        }
        break;
    }
}

Client::Client(Shared * shared) : m_listenThread(&Client::listen, this), m_shared(shared), m_waitPacket(false)//, m_inputLatency(0.05), m_inputLatencyTime(0)
{
    m_connected = false;
    m_serverIp = sf::IpAddress::getLocalAddress();
    m_serverPort = int(Network::ServerPort);
    // m_serverIp = sf::IpAddress::getPublicAddress();
    std::cout << "Twoj adres IP: " <<sf::IpAddress::getLocalAddress() << ":" << int(Network::ClientsPort) <<std::endl;
    std::cout << "Server IP: " << m_serverIp << ":" << m_serverPort <<std::endl;
    setup(&handlePacket, this);
}

Client::~Client()
{
    m_socket.unbind();
}

bool Client::disconnect()
{
    std::cout<<"Disconnecting... \n";

    m_shared->m_stateManager->switchTo(StateType::Intro);

    if (!m_connected)
        return false;

    sf::Packet packet;
    stampPacket(PacketType::Disconnect, packet);
    sf::Socket::Status status = m_socket.send(packet, m_serverIp, m_serverPort);
    m_connected = false;
    m_socket.unbind(); // Unbind to close the listening thread.

    if (status != sf::Socket::Done)
        return false;

    return true;
}

void Client::listen()
{
    sf::Packet packet;
    sf::IpAddress clientIP;
    sf::Uint16 recvPORT;

    while (m_connected)
    {
        packet.clear();

        sf::Socket::Status status = m_socket.receive(packet, clientIP, recvPORT);

        if (status != sf::Socket::Done)
        {
            if (m_connected)
            {
                std::cout << "Failed receiving a packet from "
                          << clientIP << ":" << recvPORT << ". Status: "
                          << status << std::endl;
                continue;
            }
            else
            {
                std::cout << "Socket unbound." << std::endl;
                break;
            }
        }
        if (clientIP != m_serverIp) // Ignore packets not sent from the server.
        {
            continue;
        }

        sf::Uint16 packet_id;
        if (!(packet >> packet_id))
        {
            continue;
        }
        PacketType id = (PacketType)packet_id;
        if (id < PacketType::Disconnect || id >=PacketType::OutOfBounds)
            continue; // Invalid packet type.

        if (id == PacketType::Heartbeat)
        {
            sf::Packet packetToSend;

            stampPacket(PacketType::Heartbeat, packetToSend);
            if (m_socket.send(packetToSend, m_serverIp, m_serverPort) != sf::Socket::Done)
                std::cout << "Failed sending a heartbeat!" << std::endl;

            sf::Int32 timestamp;
            packet >> timestamp;
            setTime(sf::milliseconds(timestamp));
            m_lastHeartbeat = m_serverTime;
        }
        else if(m_packetHandler)
        {
            m_packetHandler((sf::Uint16)id, packet, this); // Handle.
        }
    }
}

void Client::setTime(const sf::Time & time)
{
    m_serverTime = time;
}

bool Client::send(sf::Packet& packet)
{
    if (!m_connected)
        return false;

    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
        return false;

    return true;
}

void Client::setup(void(*handler)(const sf::Uint16&, sf::Packet&, Client*))
{
    m_packetHandler = std::bind(handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Client::unregisterPacketHandler()
{
    m_packetHandler = nullptr;
}

void Client::update(const float & time)
{
    if (!m_connected)
        return;

    m_dTime = time;
    //m_inputLatencyTime += time;
    m_serverTime += sf::seconds(time);
    if (m_serverTime.asMilliseconds() < 0)
    {
        m_serverTime -= sf::milliseconds( sf::Int32(Network::HighestTimestamp) );
        m_lastHeartbeat = m_serverTime;
        return;
    }

    if (m_serverTime.asMilliseconds() - m_lastHeartbeat.asMilliseconds() >= sf::Int32(Network::ClientTimeout))
    {
        std::cout << "Server connection timed out!" << std::endl;
        disconnect();
    }
}

Entity * Client::getPlayer() { return m_player; }
void stampPacket(const PacketType& type, sf::Packet& packet) {packet << sf::Uint16(type);}

bool Client::isWaiting() { return m_waitPacket; }

void Client::addUnknownEntity(const int & id)
{
    for( auto element : m_unknownEntities)
        if(element == id)
            return;


    m_unknownEntities.push_back(id);
}

sf::Uint8 Client::connect(std::string username, std::string password)
{
    if (m_connected)
        return 1;

    sf::Packet packet;
#ifdef __DEBUG

    if( !m_socket.bind( int(Network::ClientsPort) ))
       if(m_socket.bind( int(Network::ClientsPort) + 1))
        cout << "Podlaczono do " << int(Network::ClientsPort) + 1 << endl;
#else
    m_socket.bind( int(Network::ClientsPort) );
#endif // __DEBUG

    stampPacket(PacketType::Connect, packet);
    packet << username;
    packet << password;
    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
    {
        m_socket.unbind();
        return 2;
    }
    m_socket.setBlocking(false);
    packet.clear();

    sf::IpAddress clientIP;
    sf::Uint16 recvPORT;
    sf::Clock timer;
    timer.restart();

    #ifdef __DEBUG
    while (timer.getElapsedTime().asMilliseconds() < 500 )
    #else
    while (timer.getElapsedTime().asMilliseconds() < CONNECT_TIMEOUT )
    #endif // __DEBUG
    {

        sf::Socket::Status status = m_socket.receive(packet, clientIP, recvPORT);
        if (status != sf::Socket::Done)
            continue;

        if (clientIP != m_serverIp) //ignore packet not from server
            continue;


        sf::Uint16 id;
        if (!(packet >> id))
            break;

        if ((PacketType)id != PacketType::Connect)
            continue;

        bool result;
        packet >> result;
        if(result == 0)
        {
            m_packetHandler((sf::Uint16)id, packet, this);

            m_connected = true;
            m_socket.setBlocking(true);
            m_lastHeartbeat = m_serverTime;
            m_listenThread.launch();
            return 0;
        }
        else if (result == 1) return 5;
        else return 4;
    }
    std::cout << "Connection attempt failed! Server info: " << m_serverIp << ":" << m_serverPort << std::endl;
    m_socket.unbind();
    m_socket.setBlocking(true);
    return 3;
}

void Client::getCharacterList()
{
    if (!m_connected)
        return;

    sf::Packet packet;
    stampPacket(PacketType::GetCharacterList, packet);
    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
    {
        #ifdef __DEBUG
        std::cout << "Failed to receive character list! \n";
        #endif // __DEBUG
    }
    m_waitPacket = true;
}

void Client::enterWorld(const sf::Uint64 & cid)
{
    if (!m_connected)
        return;

    sf::Packet packet;
    stampPacket(PacketType::EnterWorld, packet);
    packet << cid;
    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
    {
        #ifdef __DEBUG
        std::cout << "Failed to enter world! \n";
        #endif // __DEBUG
    }
    m_waitPacket = true;
}

void Client::move(const int & x, const int & y)
{
    if (!m_connected)
        return;

    m_player->dx = x;
    m_player->dy = y;

    sf::Packet packet;
    stampPacket(PacketType::Move, packet);
    packet << m_player->dx << m_player->dy;
    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
    {
        #ifdef __DEBUG
        std::cout << "Failed to send packet! \n";
        #endif // __DEBUG
    }
}

void Client::getCharacterDetails()
{
    sf::Packet packet;
    stampPacket(PacketType::GetCharacterDetails, packet);

    for( auto element : m_unknownEntities)
    {
        packet << (sf::Int32)element;
    }

    if (m_socket.send(packet, m_serverIp, m_serverPort) != sf::Socket::Done)
    {
        #ifdef __DEBUG
        std::cout << "Failed to send packet! \n";
        #endif // __DEBUG
    }
    m_unknownEntities.clear();
}
