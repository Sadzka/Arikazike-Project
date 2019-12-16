#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Shared.hpp"
#include <cmath>
class Item : public Object
{
    sf::Sprite sprite;
    int type;
    float elapsedTime;
    float dTime;
public:

    Item(const int &type, sf::Vector2f pos, Shared * shared);
    void draw(sf::RenderWindow * window);

    bool contains(sf::Vector2f pos, float pickupRange);
    void update(const float &dTime);

};
