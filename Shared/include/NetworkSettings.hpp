#pragma once

#define HEARTBEAT_INTERVAL 1000 // Milliseconds.
#define HEARTBEAT_RETRIES 5
#define SNAPSHOT_INTERVAL 100

using ClientID = int;
using PortNumber = unsigned short;

enum class Network
{
    HighestTimestamp = 2147483647, //ms
    ClientTimeout = 7500,
    ServerPort = 5600,
    ClientsPort = 49787,
    NullID = -1,
    PlayerUpdateDelim = -1
};




/*
sf::Packet& operator >>(sf::Packet& packet, const Entity& entity)
{
    return packet >> entity.getName()
                  >> sf::Uint32( entity.getId() )
                  >> sf::Uint8( entity.getType() )
                  >> float(entity.getPosition().x)
                  >> float(entity.getPosition().y);
}
*/
