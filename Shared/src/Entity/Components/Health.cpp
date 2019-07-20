#include "Entity/Components/Health.hpp"
/*
CHealth::CHealth() : m_dead(false), m_health(10), m_healthMax(10)
{
    m_type = ComponentType::Health;
}

void CHealth::round()
{
    if(m_health > m_healthMax)
        m_health = m_healthMax;

    if(m_health <= 0)
    {
        m_health = 0;
        m_dead = true;
    }
}

sf::Int64 CHealth::getHealth() { return m_health; }
sf::Int64 CHealth::getHealthMax() { return m_healthMax; }

void CHealth::setHealth(const sf::Int64& health) { m_health = health; }
void CHealth::setHealthMax(const sf::Int64& health) { m_healthMax = health; }

void CHealth::damage(const sf::Int64& damage)
{
    m_health -= damage;
}

void CHealth::resurrect(const float& health)
{
    m_dead = false;
    m_health = health* m_healthMax;
}
*/
