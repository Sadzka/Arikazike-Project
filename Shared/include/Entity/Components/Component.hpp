#pragma once

enum class ComponentType
{
    Health,
    Mana,
    Stats

};

class Component
{

protected:
    ComponentType   m_type;

public:
    ComponentType getComponentType() { return m_type; }
};
