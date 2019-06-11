#include "Entity/EntitySnapshot.hpp"

sf::Packet& operator << (sf::Packet& packet, const EntitySnapshot& snapshot)
{
    return packet << snapshot.position.x
                  << snapshot.position.y
                  << snapshot.dx
                  << snapshot.dy;
}

sf::Packet& operator >> (sf::Packet& packet, EntitySnapshot& snapshot)
{
    packet  >> snapshot.position.x
            >> snapshot.position.y
            >> snapshot.dx
            >> snapshot.dy;

    return packet;
}
