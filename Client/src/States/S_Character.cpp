#include "States/S_Character.hpp"

S_Character::S_Character(StateManager* stateManager) : State(stateManager), m_select(-1) { }
S_Character::~S_Character() { }

void S_Character::loadGUI()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    auto picture = tgui::Picture::create( "Data\\img\\Screens\\login.jpg" );
    picture->setSize("100% + 1", "100% + 1");
    gui->add(picture);

    // Create the Enter button
    auto buttonEnter = tgui::Button::create("Enter World");
    buttonEnter->setSize({300, 40});
    buttonEnter->setPosition({"50% - 150", "100% - 60"});
    buttonEnter->setTextSize(24);
    gui->add(buttonEnter, "Enter");
    // Create the Logout button
    auto buttonLogout = tgui::Button::copy(buttonEnter);
    buttonLogout->setText("Logout");
    buttonLogout->setPosition({"10", "100% - 60"});
    gui->add(buttonLogout, "Logout");
    // Create the Create button
    auto buttonCreate = tgui::Button::copy(buttonEnter);
    buttonCreate->setText("Create");
    buttonCreate->setPosition({"100% - 310", "100% - 60"});
    gui->add(buttonCreate, "Create");

    // C Create character State
/*
    auto buttonCreate = tgui::Button::copy(buttonEnter);
    buttonLogout->setText("Create");
    gui->add(buttonEnter, "CCreate");

    auto buttonLogout = tgui::Button::copy(buttonLogout);
    buttonLogout->setText("Back");
    gui->add(buttonLogout, "CBack");
*/

    /// /// ///

    auto panelTemplate = tgui::Button::create( "" );
    panelTemplate->setSize( {200, 50} );
    panelTemplate->getRenderer()->setBorderColor(sf::Color::Black);

    while(m_stateMgr->getContext()->m_client->isWaiting()) { };

    int i = 0;
    for(auto object : m_stateMgr->getContext()->m_entityManager->getEntities())
    {
        auto panel = tgui::Button::copy(panelTemplate);
        panel->setText( object.second->getName() );
        panel->setPosition( {10 + 210*i, 10} );
        panel->connect( "pressed", &selectCharacter, this, i, object.second->getId() );
        gui->add(panel, "Panel" + std::to_string(i) );
        i++;
    }
}

void S_Character::onCreate()
{
    m_circleSelect.setRadius(10);
    m_circleSelect.setFillColor(sf::Color::Red);
    m_circleSelect.setPosition( {25 + 210*m_select, 25}  );
}

void S_Character::onDestroy() {}

void S_Character::update(float & dTime)
{
    m_stateMgr->getContext()->m_client->update(dTime);
}

void S_Character::draw()
{
    if(m_select != -1)
        m_stateMgr->getContext()->m_wind->draw(m_circleSelect);
}

void S_Character::activate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    m_stateMgr->getContext()->m_entityManager->purge();
    m_stateMgr->getContext()->m_client->getCharacterList();
    loadGUI();

    //login button
    auto widget = gui->get("Logout");
    if(widget)
        widget->connect( "pressed", [&]()
        {
            m_stateMgr->getContext()->m_client->disconnect();
        } );

    //Enter World button
    widget = gui->get("Enter");
    if(widget)
        widget->connect( "pressed", &enterWorld, this);

    widget = gui->get("Create");
    if(widget)
        widget->connect( "pressed", [&]()
        {
            /*
            //m_stateMgr->getContext()->m_gui->get("Logout")->hideWithEffect( tgui::ShowAnimationType::Fade, sf::seconds(0) );
            m_stateMgr->getContext()->m_gui->get("Logout")->setVisible(false);
            m_stateMgr->getContext()->m_gui->get("Create")->setVisible(false);
            m_stateMgr->getContext()->m_gui->get("Enter")->setVisible(false);
            for (int i = 0; i < m_stateMgr->getContext()->m_entityManager->getSize(); i++ )
                m_stateMgr->getContext()->m_gui->get("Panel" + std::to_string(i))->setVisible(false);
            */
        });

}

void S_Character::deactivate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;
    gui->removeAllWidgets();
}

void S_Character::selectCharacter(const int & select, const sf::Uint64 & cid)
{
    m_select = select;
    m_cid = cid;
    m_circleSelect.setPosition( {25 + 210*m_select, 25}  );
}

void S_Character::enterWorld()
{
    if(m_select != -1)
    {
        m_stateMgr->getContext()->m_client->enterWorld( m_cid );
        m_stateMgr->getContext()->m_entityManager->purge();
        m_stateMgr->switchTo(StateType::Game);
    }
}
