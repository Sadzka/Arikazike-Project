#include "States/S_Game.hpp"

S_Game::S_Game(StateManager* stateManager) : State(stateManager) {}

S_Game::~S_Game() { }

void S_Game::loadGUI()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
    /// menu pod escape

    auto item1 = tgui::Button::create( "" );
    item1->setSize( {64, 64} );
    item1->setPosition({"50% - 32" , "100% - 64"});
    item1->getRenderer()->setTexture("Data/img/Items/item1.png");
    item1->connect("pressed", [&]()
    {
        m_stateMgr->getContext()->m_gui->get<tgui::Button>("MainSlot1")->getRenderer()->setTexture("Data/img/Items/item1s.png");
        m_stateMgr->getContext()->m_gui->get<tgui::Button>("MainSlot2")->getRenderer()->setTexture("Data/img/Items/item2.png");
        m_stateMgr->getContext()->m_client->getPlayer()->getSpriteSheetWeapon().changeTexture("Pickaxe.png");
    });
    gui->add(item1, "MainSlot1" );

    auto item2 = tgui::Button::create( "" );
    item2->setSize( {64, 64} );
    item2->setPosition({"50% + 32" , "100% - 64"});
    item2->getRenderer()->setTexture("Data/img/Items/item2.png");
    item2->connect("pressed", [&]()
    {
        m_stateMgr->getContext()->m_gui->get<tgui::Button>("MainSlot1")->getRenderer()->setTexture("Data/img/Items/item1.png");
        m_stateMgr->getContext()->m_gui->get<tgui::Button>("MainSlot2")->getRenderer()->setTexture("Data/img/Items/item2s.png");
        m_stateMgr->getContext()->m_client->getPlayer()->getSpriteSheetWeapon().changeTexture("Axe.png");
    });
    gui->add(item2, "MainSlot2" );


}

void S_Game::onCreate()
{
    //EventManager* evMgr = m_stateMgr->getContext()->m_eventManager;

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
}

void S_Game::draw()
{
    m_view.setCenter( m_stateMgr->getContext()->m_client->getPlayer()->getPosition() );
    if(m_view.getCenter().x - m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().x/2 < 0)
        m_view.setCenter( sf::Vector2f(m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().x/2, m_view.getCenter().y) );
    else if(m_view.getCenter().x + m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().x/2
            > m_stateMgr->getContext()->m_gameMap->getmapsize().x*64)
        m_view.setCenter( sf::Vector2f(m_stateMgr->getContext()->m_gameMap->getmapsize().x*64 - m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().x/2, m_view.getCenter().y) );

    if(m_view.getCenter().y - m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().y/2 < 0)
        m_view.setCenter( sf::Vector2f(m_view.getCenter().x, m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().y/2) );
    else if(m_view.getCenter().y + m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().y/2
            > m_stateMgr->getContext()->m_gameMap->getmapsize().y*64)
        m_view.setCenter( sf::Vector2f(m_view.getCenter().x, m_stateMgr->getContext()->m_gameMap->getmapsize().y*64 - m_stateMgr->getContext()->m_wind->getRenderWindow()->getSize().y/2) );

    m_stateMgr->getContext()->m_wind->getRenderWindow()->setView(m_view );

    m_stateMgr->getContext()->m_gameMap->draw();

    m_stateMgr->getContext()->m_entityManager->draw();

    ///
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
    gui->draw();
}

void S_Game::activate()
{
    while(m_stateMgr->getContext()->m_client->isWaiting()) { /* Do Nothing */};

    loadGUI();
    //tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
}

void S_Game::deactivate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
    gui->removeAllWidgets();
}

void S_Game::move(EventDetails* details)
{
    if(!m_stateMgr->getContext()->m_wind->getRenderWindow()->hasFocus()) return;


    m_stateMgr->getContext()->m_client->attack( sf::Mouse::isButtonPressed( sf::Mouse::Left ) );

    int dx = 0;
    int dy = 0;
    //if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( m_stateMgr->getContext()->m_keybinds->find( (int)Action::MoveLeft )->first) ) )
    //    dx--;
    std::vector < sf::Keyboard::Key > binds = *m_stateMgr->getContext()->m_keybinds;

    if( sf::Keyboard::isKeyPressed( binds[(int)Action::MoveUp] ) )
        dy--;
    if( sf::Keyboard::isKeyPressed( binds[(int)Action::MoveDown] ) )
        dy++;
    if( sf::Keyboard::isKeyPressed( binds[(int)Action::MoveLeft] ) )
        dx--;
    if( sf::Keyboard::isKeyPressed( binds[(int)Action::MoveRight] ) )
        dx++;

    if( m_stateMgr->getContext()->m_gameMap->isCollision(dx, dy) )
    {
        dx = 0;
        dy = 0;
    }

    m_stateMgr->getContext()->m_client->move(dx, dy);

}
