#include "Map/Tile.hpp"

Tile::Tile(const sf::Texture & texture, const sf::IntRect & rect, int id)
{
    m_id = id;
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(rect);
}
