#pragma once
#include <iostream>
#include "Window.hpp"
#include "Shared.hpp"
#include "Spritesheet/SpriteSheet.hpp"
#include "Map/Map.hpp"
#include "Server/Client.hpp"
#include "Entity/C_EntityManager.hpp"
#include "Entity/Components/CStats.hpp"
#include "Entity/Components/CMana.hpp"


class Game
{
    ///fps limit
    sf::Time            m_FPS;
    sf::Uint16          m_maxFPS;
    float               m_frametime;
    ///
    sf::Clock           m_clock;
    sf::Time            m_deltaTime;

    tgui::Gui           m_gui;
    Shared              m_shared;
    Window              m_window;
    StateManager        m_stateManager;
    TextureManager      m_textureManager;
    C_EntityManager     m_entityManager;
    Map                 m_map;
    Client              m_client;
public:
    Game();
    ~Game();

    void FPSLimit();
    void handleInput();
    void update();
    void lateUpdate();
    void render();

    sf::Time getElapsed();
    void restartClock();

    Window* getWindow();

    ///
    void moveTest(EventDetails* details);
};
