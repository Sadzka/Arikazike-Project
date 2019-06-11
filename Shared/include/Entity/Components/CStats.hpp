#pragma once
#include <SFML/Config.hpp>
#include "../../Shared/include/Entity/Components/Component.hpp"
#include <SFML/System.hpp>

enum class damageType
{
    Fire = 0,
    Frost = 1,
    Shadow = 2,
    Arcane = 3,
    Nature = 4,
    Physical = 5,
    Holy = 6,
    Unholy = 7,
    Chaos = 8
};

class CStats : public Component
{
    sf::Uint16 m_stamina;
    sf::Uint16 m_strength;
    sf::Uint16 m_agility;
    sf::Uint16 m_intelect;
    sf::Uint16 m_spirit;

    sf::Uint16 m_crit;
    sf::Uint16 m_haste;
    sf::Uint16 m_mp5;
    sf::Uint16 m_armorPenetration;
    sf::Uint16 m_spellPenetration;

    sf::Uint16 m_hit;
    sf::Uint16 m_expertise;

    sf::Uint16 m_armor;
    sf::Uint16 m_dodge;
    sf::Uint16 m_block;
    sf::Uint16 m_parry;
    sf::Uint16 m_resistance[5];


    sf::Uint16 m_attackPower;
    sf::Uint16 m_spellPower;
public:
    /// \todo wtf xD
    update( const sf::Uint16 & stamina, const sf::Uint16 & strength, const sf::Uint16 & agility, const sf::Uint16 & intelect, const sf::Uint16 & spirit,
            const sf::Uint16 & crit, const sf::Uint16 & haste,const sf::Uint16 & mp5, const sf::Uint16 & armorPenetration, const sf::Uint16 & spellPenetration,
            const sf::Uint16 & hit, const sf::Uint16 & expertise, const sf::Uint16 & attackPower, const sf::Uint16 & spellPower,
            const sf::Uint16 & armor, const sf::Uint16 & dodge, const sf::Uint16 & block, const sf::Uint16 & parry,
            const sf::Uint16 & resistance0, const sf::Uint16 & resistance1, const sf::Uint16 & resistance2, const sf::Uint16 & resistance3, const sf::Uint16 & resistance4);
};
