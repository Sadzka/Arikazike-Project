#pragma once
#include <SFML/Graphics/Sprite.hpp>

enum Sheet
{
    Tile_Size = 32,
    Sheet_Width = 256,
    Sheet_Height = 256
};

struct Tile
{
    unsigned int m_id;
    sf::Sprite m_sprite;
    Tile(const sf::Texture & texture, const sf::IntRect & rect, int id);
};
