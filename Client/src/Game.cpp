#include "Game.hpp"

Game::Game() : m_window("Arikazike", sf::Vector2u(1366, 768) ),
               m_stateManager(&m_shared), m_entityManager(&m_shared), m_client(&m_shared), m_map(&m_shared )
{
    m_shared.m_wind             = &m_window;
    m_shared.m_stateManager     = &m_stateManager;
    m_shared.m_textureManager   = &m_textureManager;
    m_shared.m_gui              = &m_gui;
    m_shared.m_eventManager     = m_window.getEventManager();
    m_shared.m_gui->setTarget( *m_window.getRenderWindow() );
    m_shared.m_client           = &m_client;
    m_shared.m_entityManager    = &m_entityManager;
    m_shared.m_gameMap          = &m_map;

    //std::cout << " Entity manager id: "<< <<"\n\n";

    m_stateManager.switchTo(StateType::Intro);

    /// \todo
    /// Temporary
    m_maxFPS = 30;
    m_frametime = 1.0/m_maxFPS;
    ///
}

Game::~Game()
{
    m_shared.m_client->disconnect();
}

void Game::update()
{
    float time = m_deltaTime.asSeconds();
    m_window.update( &m_gui );
    m_stateManager.update(time);
}

void Game::render()
{
    m_window.beginDraw();

    m_gui.draw();
    m_stateManager.draw();


    m_window.endDraw();
}

void Game::lateUpdate()
{
    m_stateManager.processRequests();
    restartClock();
}

void Game::restartClock()
{
    m_deltaTime = m_clock.restart();
    FPSLimit();
}

void Game::FPSLimit()
{
    m_FPS += getElapsed();

    if(m_FPS < sf::seconds(m_frametime))
        sf::sleep( sf::seconds(m_frametime - m_FPS.asSeconds()) );

    m_FPS -= sf::seconds(m_frametime);
}

sf::Time Game::getElapsed(){ return m_deltaTime; }
Window* Game::getWindow() { return &m_window; }
