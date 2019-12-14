#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "../../../Shared/include/Entity/Directions.hpp"
#include "Spritesheet/Animation.hpp"
#include "ResourcesManager/TextureManager.hpp"

using Animations = std::unordered_map<std::string, Animation*>;

class SpriteSheet
{
    std::string         m_texture;
    sf::Sprite          m_sprite;
    sf::Vector2i        m_spriteSize;
    sf::Vector2f        m_spriteScale;
    Direction           m_direction;
    Animations          m_animations;
    Animation         * m_animationCurrent;
    TextureManager    * m_textureManager;

public:
    SpriteSheet(TextureManager* textMgr);
    ~SpriteSheet();
    void cropSprite(const sf::IntRect& rect);

    bool loadSheet(const std::string& file);
    void releaseSheet();

    sf::Vector2i getSpriteSize()const;
    sf::Vector2f getSpritePosition()const;
    Direction getDirection()const;
    Animation * getCurrentAnim();

	void setSpriteSize(const sf::Vector2u& size);
	void setSpritePosition(const sf::Vector2f& pos);
	void setDirection(const Direction& dir);

    bool setAnimation(const std::string& name, const bool& play = false, const bool& loop = false);
    void setSpriteSize(const sf::Vector2i& size);
    void setAlpha(const sf::Uint8 & alpha);

    void update(const float & dTime);
    void draw(sf::RenderWindow* window);

    void changeTexture(const std::string &name);
};
