#include "PacketHandler.hpp"

void Handler(sf::IpAddress& ip, const PortNumber& port, const PacketID& pid, sf::Packet& packet, Server* server)
{
    ClientID id = server->getClientID(ip, port);
    sf::Packet packetToSend;
    if (id >= 0) // client connected
    {
        if((PacketType)pid == PacketType::Connect)
        {
            stampPacket(PacketType::Disconnect, packetToSend);
            server->send(ip, port, packetToSend);
#ifdef __DEBUG
            cout << "Proba zalogowania na polaczone konto " << id
                 << " " << server->findClient(id)->m_username
                 << " " << server->findClient(id)->m_clientIP << endl;
#endif // __DEBUG
        }
        else if((PacketType)pid == PacketType::Disconnect)
        {
            /// \todo ///
            server->removeClient(ip, port);
            stampPacket(PacketType::Message, packetToSend);
            std::string message;
            message = "Client left! " + ip.toString() + ":" + std::to_string(port) + " ID: " + std::to_string(id);
            std::cout << message << std::endl;
            //packetToSend << message;
            server->send(ip, port, packetToSend);
        }
        else if ((PacketType)pid == PacketType::Message)
        {
            /// \todo ///

            std::string receivedMessage;
            packet >> receivedMessage;
            std::string message = ip.toString() + ":" +
                                  std::to_string(port) + " :" + receivedMessage;
            stampPacket(PacketType::Message, packetToSend);
            packetToSend << message;
            server->broadcast(packetToSend, id);
        }
        else if ((PacketType)pid == PacketType::GetCharacterList)
        {
#ifdef __DEBUG
            cout << "Otrzymanie listy postaci " << endl;
#endif // __DEBUG

            stampPacket(PacketType::GetCharacterList, packetToSend);

            server->mysql._GetCharacterList(id, packetToSend);
            server->send(ip, port, packetToSend);
        }
        else if ((PacketType)pid == PacketType::EnterWorld)
        {

            ClientInfo * client = server->findClient(id);
            if(client->m_character != nullptr)
                return; //already in game, ignore

            sf::Uint64 characterID;
            packet>>characterID;

#ifdef __DEBUG
            cout << "Wejscie do swiata " << characterID << endl;
#endif // __DEBUG

            stampPacket(PacketType::GetCharacterDetails, packetToSend);
            server->mysql._GetCharacter(client, characterID, packetToSend);
            server->send(ip, port, packetToSend);
        }
        else if ((PacketType)pid == PacketType::Move)
        {
            sf::Int8 x, y;
            sf::Int32 hearhbeat;

            packet >> hearhbeat;
            packet >> x >> y;

            Entity * unit = server->findClient(id)->m_character;

            if(unit != nullptr)
            {
                if( unit->getLastHearhbeat() < hearhbeat )
                {
                    unit->setMoving(x, y);
                }
            }

        }
        else if ((PacketType)pid == PacketType::GetCharacterDetails)
        {
            stampPacket(PacketType::GetCharacterDetails, packetToSend);
            sf::Uint32 id = -1;
            while(packet >> id)
            {
                Entity* entity = server->findEntity(id);
                if(entity != nullptr)
                    packetToSend << *entity;
            }
            server->send(ip, port, packetToSend);
        }

        return;
    }

    //Client not found
    if ((PacketType)pid == PacketType::Connect)
    {
        std::string username;
        std::string password;

        packet >> username >> password;

        int result = server->mysql._Connect(username, password);

        if( result != -1  )
        {

            ClientInfo client(ip, port, server->getServerTime(), username, result);

            if(server->addClient(client) != -1)
            {
                stampPacket(PacketType::Connect, packetToSend);
                packetToSend << (bool)(0);
                std::cout << "New client from IP: " << ip << ":" << port << " ID: " << std::to_string(client.m_clientID) << "\n";
            }
            else
            {
                stampPacket(PacketType::Connect, packetToSend);
                packetToSend << (bool)(1);
                std::cout << "Failed login from IP: " << ip << ":" << port  << " already logged in\n";
            }
        }
        else
        {
            stampPacket(PacketType::Connect, packetToSend); /// \todo failed login
            packetToSend << (bool)(2);
            std::cout << "Failed login from IP: " << ip << ":" << port  << "\n";
        }

        server->send(ip, port, packetToSend);
    }
}

void stampPacket(const PacketType& type, sf::Packet& packet)
{
    packet << sf::Uint16(type);
}
