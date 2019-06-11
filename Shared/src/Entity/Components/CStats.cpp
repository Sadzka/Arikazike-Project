#include "../../Shared/include/Entity/Components/CStats.hpp"

CStats::update( const sf::Uint16 & stamina, const sf::Uint16 & strength, const sf::Uint16 & agility, const sf::Uint16 & intelect, const sf::Uint16 & spirit,
            const sf::Uint16 & crit, const sf::Uint16 & haste,const sf::Uint16 & mp5, const sf::Uint16 & armorPenetration, const sf::Uint16 & spellPenetration,
            const sf::Uint16 & hit, const sf::Uint16 & expertise, const sf::Uint16 & attackPower, const sf::Uint16 & spellPower,
            const sf::Uint16 & armor, const sf::Uint16 & dodge, const sf::Uint16 & block, const sf::Uint16 & parry,
            const sf::Uint16 & resistance0, const sf::Uint16 & resistance1, const sf::Uint16 & resistance2, const sf::Uint16 & resistance3, const sf::Uint16 & resistance4)
{
    m_stamina = stamina;
    m_strength = strength;
    m_agility = agility;
    m_intelect = intelect;
    m_spirit = spirit;

    m_crit = crit;
    m_haste = haste;
    m_mp5 = mp5;
    m_armorPenetration = armorPenetration;
    m_spellPenetration = spellPenetration;

    m_hit = hit;
    m_expertise = expertise;

    m_armor = armor;
    m_dodge = dodge;
    m_block = block;
    m_parry = parry;
    m_resistance[0] = resistance0;
    m_resistance[1] = resistance1;
    m_resistance[2] = resistance2;
    m_resistance[3] = resistance3;
    m_resistance[4] = resistance4;


    m_attackPower = attackPower;
    m_spellPower = spellPower;
}
