#include "States/S_Game.hpp"

S_Game::S_Game(StateManager* stateManager) : State(stateManager) {}

S_Game::~S_Game() { }

void S_Game::loadGUI()
{

}

void S_Game::onCreate()
{
    EventManager* evMgr = m_stateMgr->getContext()->m_eventManager;

/*
    evMgr->addCallback(StateType::Game, "A", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "D", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "S", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "W", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "AR", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "DR", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "SR", &S_Game::move, this);
    evMgr->addCallback(StateType::Game, "WR", &S_Game::move, this);
*/
    m_stateMgr->getContext()->m_gameMap->loadMap( "GMIsland.txt" );
}

void S_Game::onDestroy()
{
    /*
    EventManager* evMgr = m_stateMgr->getContext()->m_eventManager;
    evMgr->removeCallback(StateType::Game, "A" );
    evMgr->removeCallback(StateType::Game, "D" );
    evMgr->removeCallback(StateType::Game, "S" );
    evMgr->removeCallback(StateType::Game, "W" );
    evMgr->removeCallback(StateType::Game, "AR" );
    evMgr->removeCallback(StateType::Game, "DR" );
    evMgr->removeCallback(StateType::Game, "SR" );
    evMgr->removeCallback(StateType::Game, "WR" );
    */
}

void S_Game::update(float & dTime)
{
    m_stateMgr->getContext()->m_entityManager->update(dTime);
    m_stateMgr->getContext()->m_client->update(dTime);
    move(nullptr);

/*
    Client * client = m_stateMgr->getContext()->m_client;
    int dx = 0;
    int dy = 0;

    if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
        dx--;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
        dx++;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
        dy--;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
        dy++;

    client->move(dx, dy);
    */
}

void S_Game::draw()
{
    m_view.setCenter( m_stateMgr->getContext()->m_client->getPlayer()->getPosition() );
    m_stateMgr->getContext()->m_wind->getRenderWindow()->setView(m_view );

    m_stateMgr->getContext()->m_gameMap->draw();

    m_stateMgr->getContext()->m_entityManager->draw();
}

void S_Game::activate()
{

    while(m_stateMgr->getContext()->m_client->isWaiting()) { };

    loadGUI();
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

}

void S_Game::deactivate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
    gui->removeAllWidgets();
}

void S_Game::move(EventDetails* details)
{

    int dx = 0;
    int dy = 0;

    if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
        dx--;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
        dx++;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
        dy++;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
        dy--;

/*
    if(details->m_name == "A" )
        dx--;
    else if(details->m_name == "D" )
        dx++;
    else if(details->m_name == "S" )
        dy++;
    else if(details->m_name == "W" )
        dy--;

    if(details->m_name == "SR" || details->m_name == "WR" )
    {
        dy = 0;
    }
    else if(details->m_name == "DR" || details->m_name == "AR" )
    {
        dx = 0;
    }
*/
    m_stateMgr->getContext()->m_client->move(dx, dy);

}
