#include "Window.hpp"

void Window::setup(const std::string& title, const sf::Vector2u& size)
{
    /// \todo (Sadza#1#04/17/19):
    /// Temporary
    m_window.setFramerateLimit(30);
    ///

    m_isFullscreen = false;
    m_isClosed = false;
    m_title = title;
    m_isFocused = true;
    m_eventManager.addCallback(StateType(0), "Fullscreen_toggle", &Window::toggleFullscreen, this);
    m_eventManager.addCallback(StateType(0), "Window_close", &Window::close, this);

    create(size);
}

void Window::create(const sf::Vector2u& size)
{
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create( sf::VideoMode(size.x, size.y, 32), m_title, style);
}

void Window::destroy() { m_window.close(); }

Window::Window() { setup("Window", sf::Vector2u(800, 600)); }
Window::Window(const std::string& title, const sf::Vector2u& size) { setup(title, size); }
Window::~Window() { destroy(); }

void Window::update(tgui::Gui * gui)
{
    while(m_window.pollEvent(event))
    {
        if (event.type == sf::Event::LostFocus)
        {
            m_isFocused = false;
            m_eventManager.setFocus(false);
        }
        else if (event.type == sf::Event::GainedFocus)
        {
            m_isFocused = true;
            m_eventManager.setFocus(true);
        }

        gui->handleEvent(event);
        m_eventManager.handleEvent(event);
    }
    m_eventManager.update();
}

void Window::toggleFullscreen(EventDetails* details)
{
    m_isFullscreen = !m_isFullscreen;
    destroy();
    create(m_window.getSize());
}

void Window::close(EventDetails* details) { m_isClosed = true; }

sf::Vector2u Window::getWindowSize() { return m_window.getSize(); }
EventManager* Window::getEventManager() { return &m_eventManager; }
sf::RenderWindow * Window::getRenderWindow() { return &m_window; }
sf::FloatRect Window::getViewSpace()
{
    sf::Vector2f viewCenter = m_window.getView().getCenter();
    sf::Vector2f viewSize = m_window.getView().getSize();
    sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
    sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
    return viewSpace;
}
bool Window::isClosed() { return m_isClosed; }
bool Window::isFullscreen() { return m_isFullscreen; }

void Window::beginDraw(){ m_window.clear(sf::Color::Black); }
void Window::endDraw(){ m_window.display(); }
void Window::draw(sf::Drawable& drawable){ m_window.draw(drawable); }
