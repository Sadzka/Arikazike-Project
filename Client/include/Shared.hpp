#pragma once
#include <TGUI/TGUI.hpp>
#include "ResourcesManager/TextureManager.hpp"
#include "States/StateManager.hpp"
#include "Window.hpp"
#include "Entity/C_EntityManager.hpp"
#include "Server/Client.hpp"
#include "Map/Map.hpp"

class StateManager;
class Client;
class C_EntityManager;

struct Shared
{
    Shared() :  m_wind(nullptr),
                m_eventManager(nullptr),
                m_stateManager(nullptr),
                m_textureManager(nullptr),
                m_gameMap(nullptr),
                m_entityManager(nullptr),
                m_client(nullptr)
                {}

    tgui::Gui * m_gui;
    Window * m_wind;
    EventManager * m_eventManager;
    StateManager * m_stateManager;
    TextureManager * m_textureManager;
    C_EntityManager * m_entityManager;
    Map * m_gameMap;
    Client * m_client;

};
