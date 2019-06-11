#pragma once
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

class EntitySnapshot
{
    friend class Entity;
    friend class C_EntityManager;

    sf::Vector2f position;
    sf::Int8 dx;
    sf::Int8 dy;

    friend sf::Packet& operator<< (sf::Packet& packet, const EntitySnapshot& snapshot);
    friend sf::Packet& operator>> (sf::Packet& packet, EntitySnapshot& snapshot);
};
