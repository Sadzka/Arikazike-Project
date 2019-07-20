#pragma once
#include <SFML/Config.hpp>
#include "Entity/Components/Component.hpp"

class CMana : public Component
{
    sf::Int64          m_mana;
    sf::Int64          m_manaMax;

    void round();

public:
    CMana();

    sf::Int64 getMana();
    sf::Int64 getManaMax();

    void setMana(const sf::Int64&);
    void setManaMax(const sf::Int64&);
    void damage(const sf::Int64&);

    /// \arg mana % HP <0, 100>;
    void resurrect(const float&);
};

