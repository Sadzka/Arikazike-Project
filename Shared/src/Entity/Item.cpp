#include "../../include/Entity/Item.hpp"

Item::Item(const int &type, sf::Vector2f pos, Shared * shared) : elapsedTime(0)
{
    m_position = pos;
    this->type = type;
    sprite.setTexture( shared->m_textureManager->getResource("Data/img/Items/Resources.png") );

    if(type == 0) sprite.setTextureRect( sf::IntRect(32, 0, 32, 32) );
    else if(type == 1) sprite.setTextureRect( sf::IntRect(0, 0, 32, 32) );

    sprite.setOrigin(16,16);
    sprite.setPosition(m_position);
}

void Item::draw(sf::RenderWindow * window)
{
    if(elapsedTime < 0.33)
    {
        sprite.setPosition( sprite.getPosition().x,
                            sprite.getPosition().y - 200*dTime);
    }
    else if(elapsedTime < 1)
    {
        sprite.setPosition( sprite.getPosition().x,
                            sprite.getPosition().y + 200*dTime);
    }
    window->draw(sprite);
}

bool Item::contains(sf::Vector2f pos, float range)
{
    float dx = pos.x - sprite.getPosition().x;
    float dy = pos.y - sprite.getPosition().y;
    if( dx*dx + dy*dy < range*range)
        return true;
    return false;
}

void Item::update(const float &dTime)
{
    this->dTime = dTime;
    elapsedTime += dTime;
}
