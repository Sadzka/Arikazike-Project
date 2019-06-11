#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <TGUI/Gui.hpp>
#include "Input/EventManager.hpp"

class Window
{
    sf::RenderWindow        m_window;
    bool                    m_isClosed;
    bool                    m_isFullscreen;
    std::string             m_title;
    EventManager            m_eventManager;
    bool                    m_isFocused;
    sf::Event               event;

    void setup(const std::string& title, const sf::Vector2u& size);
    void create(const sf::Vector2u& size);
    void destroy();
public:
    Window();
    Window(const std::string& title,const sf::Vector2u& size);
    ~Window();

    void update(tgui::Gui * gui);
    void toggleFullscreen(EventDetails* details);
    void close(EventDetails* details = nullptr);

    sf::Vector2u getWindowSize();
    EventManager* getEventManager();
    sf::RenderWindow * getRenderWindow();

    /// unused ////////////// ///
    /// but may be useful /// ///
    sf::FloatRect getViewSpace();
    /// ///////////////////// ///

    bool isClosed();
    bool isFullscreen();
    bool isFocused();

    void beginDraw();
    void endDraw();
    void draw(sf::Drawable& drawable);
};
