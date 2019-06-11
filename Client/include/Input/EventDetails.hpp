#pragma once
#include <string>
#include <SFML/Graphics.hpp>

struct EventDetails
{
    EventDetails(const std::string& bindName) : m_name(bindName) { clear(); }
    std::string m_name;
    sf::Vector2i m_size;
    sf::Uint32 m_textEntered;
    sf::Vector2i m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode; // Single key code.
    void clear()
    {
        m_size = sf::Vector2i(0, 0);
        m_textEntered = 0;
        m_mouse = sf::Vector2i(0, 0);
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
    }
};
