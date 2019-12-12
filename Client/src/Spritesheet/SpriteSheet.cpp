#include "Spritesheet/SpriteSheet.hpp"

SpriteSheet::SpriteSheet(TextureManager* textMgr) : m_textureManager(textMgr), m_animationCurrent(nullptr),
	m_spriteScale(1.f, 1.f), m_direction(Direction::Down){}

SpriteSheet::~SpriteSheet(){ releaseSheet(); }

bool SpriteSheet::loadSheet(const std::string& file)
{
	std::ifstream sheet;
	sheet.open(file);
	if(sheet.is_open())
    {
		releaseSheet(); // Release current sheet resources.
		std::string line;
		while(std::getline(sheet,line))
        {
			if (line[0] == '|')
                continue;

			std::stringstream keystream(line);
			std::string type;
			keystream >> type;
			if(type == "Texture")
            {
				if (m_texture != "")
                {
					std::cerr << "! Duplicate texture entries in: " << file << std::endl;
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!m_textureManager->loadResource(texture))
                {
					std::cerr << "! Could not set up the texture: " << texture << std::endl;
					continue;
				}
				m_texture = texture;
				m_sprite.setTexture(m_textureManager->getResource( m_texture));
			}
			else if(type == "Size")
            {
				keystream >> m_spriteSize.x >> m_spriteSize.y;
				setSpriteSize(m_spriteSize);
				m_sprite.setOrigin( m_spriteSize.x/2, m_spriteSize.y/2 );
			}
			else if(type == "Scale")
            {
				keystream >> m_spriteScale.x >> m_spriteScale.y;
				m_sprite.setScale(m_spriteScale);
			}
			else if(type == "Animation")
            {
/// TODO (Sadza#1#04/18/19): ogarnac

				std::string name;
				keystream >> name;
				if (m_animations.find(name) != m_animations.end())
                {
                    #ifdef __DEBUG

					std::cerr << "! Duplicate animation(" << name << ") in: " << file << std::endl;

                    #endif // __DEBUG
					continue;
				}
				Animation * anim = new Animation;

				keystream >> *anim;
				anim->setSpriteSheet(this);
				anim->setName(name);
				anim->reset();
				m_animations.emplace(name, anim);

				if (m_animationCurrent)
                    continue;
				m_animationCurrent = anim;
				m_animationCurrent->play();
				// std::cout << m_animationCurrent->getName() << std::endl;

			}
		}
		sheet.close();
		return true;
	}
	std::cerr << "! Failed loading spritesheet: " << file << std::endl;
	return false;
}

void SpriteSheet::releaseSheet()
{
	m_animationCurrent = nullptr;
	while(m_animations.begin() != m_animations.end()){
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}
}

sf::Vector2i SpriteSheet::getSpriteSize()const{ return m_spriteSize; }
sf::Vector2f SpriteSheet::getSpritePosition()const{ return m_sprite.getPosition(); }
Direction SpriteSheet::getDirection()const{ return m_direction; }
Animation * SpriteSheet::getCurrentAnim(){ return m_animationCurrent; }

void SpriteSheet::setSpriteSize(const sf::Vector2i& size)
{
	m_spriteSize = size;
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y/2);
}

void SpriteSheet::setSpritePosition(const sf::Vector2f& pos){ m_sprite.setPosition(pos); }

void SpriteSheet::setDirection(const Direction& dir){
	if (dir == m_direction){ return; }
	m_direction = dir;
	m_animationCurrent->cropSprite();
}

void SpriteSheet::cropSprite(const sf::IntRect& rect){ m_sprite.setTextureRect(rect); }

bool SpriteSheet::setAnimation(const std::string& name, const bool& play, const bool& loop)
{
	auto itr = m_animations.find(name);
	if (itr == m_animations.end()){ return false; }
	if (itr->second == m_animationCurrent){ return false; }
	if (m_animationCurrent){ m_animationCurrent->stop(); }
	m_animationCurrent = itr->second;
	m_animationCurrent->setLooping(loop);
	if(play){ m_animationCurrent->play(); }
	m_animationCurrent->cropSprite();
	return true;
}

void SpriteSheet::setAlpha(const sf::Uint8 & alpha) { m_sprite.setColor( sf::Color(m_sprite.getColor().r, m_sprite.getColor().g, m_sprite.getColor().b, alpha) ); }

void SpriteSheet::update(const float & dTime){ m_animationCurrent->update( dTime ); }
void SpriteSheet::draw(sf::RenderWindow* window) { window->draw(m_sprite); }

