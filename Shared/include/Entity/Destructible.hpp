#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#ifdef __CLIENT
#include "Shared.hpp"
#endif // __CLIENT
#include "DestructibleTypes.hpp"

class Destructible : public Object
{
#ifdef __CLIENT
    sf::Sprite sprite;
#endif // __CLIENT

    DestructibleType type;
public:
#ifdef __CLIENT
    Destructible(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32 & id, Shared * shared);
    void draw(sf::RenderWindow * window);
#else
    Destructible(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32 & id);
#endif // __CLIENT
    sf::Uint32 getId();
    bool contains(sf::Vector2f pos);
    DestructibleType getType()const;


};


