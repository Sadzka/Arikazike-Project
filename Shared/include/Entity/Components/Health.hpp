#pragma once
#include <SFML/Config.hpp>
#include "Entity/Components/Component.hpp"

class CHealth : public Component
{
    sf::Int64          m_health;
    sf::Int64          m_healthMax;
    bool               m_dead;

    void round();

public:
    CHealth();

    sf::Int64 getHealth();
    sf::Int64 getHealthMax();

    void setHealth(const sf::Int64&);
    void setHealthMax(const sf::Int64&);
    void damage(const sf::Int64&);

    /// \arg health % HP <0, 100>;
    void resurrect(const float&);
};

