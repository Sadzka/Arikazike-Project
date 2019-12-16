#pragma once
#include <string>
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

class Object
{
protected:

    std::string m_name;
    sf::Uint32 m_id; // Entity id in the entity manager.
    sf::Vector2f m_position; // Current position.
    sf::Vector2f m_positionOld; // Position before entity moved.
public:
    virtual ~Object() {}
    sf::Vector2f getPosition() { return m_position; }
};
