#pragma once
#include <SFML/Network.hpp>
#include "NetworkSettings.hpp"
#include "Entity/Entity.hpp"

struct DestructibleInfo
{
    sf::Uint32 id;
    sf::Vector2f position;
    int type;

    sf::Vector2f size;
    //friend sf::Packet& operator<< (sf::Packet& packet, const DestructibleInfo& entity);
    friend sf::Packet& operator << (sf::Packet& packet, const DestructibleInfo& destr)
    {
        return packet   << destr.type
                        << destr.id
                        << destr.position.x
                        << destr.position.y;
    }
};


