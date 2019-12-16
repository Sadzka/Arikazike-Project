#include "../../include/Entity/Destructible.hpp"

#ifdef __CLIENT
Destructible::Destructible(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32 & id, Shared * shared)
{
    m_id = id;
    m_position = pos;
    this->type = type;
    if(type == DestructibleType::Cactus)
    {
        sprite.setTexture( shared->m_textureManager->getResource("Data/img/Destructibles/cactus.png") );
    }
    else if(type == DestructibleType::IronOre)
    {
        sprite.setTexture( shared->m_textureManager->getResource("Data/img/Destructibles/ironore.png") );
    }
    sprite.setPosition(m_position);
}
#else
Destructible::Destructible(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32 & id)
{
    this->type = type;
    m_id = id;
    m_position = pos;
}
#endif // __CLIENT

void Destructible::draw(sf::RenderWindow * window)
{
    window->draw(sprite);
}

sf::Uint32 Destructible::getId() { return m_id; }
bool Destructible::contains(sf::Vector2f pos) { return sprite.getGlobalBounds().contains(pos); }
DestructibleType Destructible::getType()const { return type; }
