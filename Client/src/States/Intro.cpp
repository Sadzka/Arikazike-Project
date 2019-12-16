#include "States/Intro.hpp"

Intro::Intro(StateManager* stateManager) : State(stateManager)
{
}

Intro::~Intro() { }

void Intro::loadGUI()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    //gui->loadWidgetsFromFile("widget.txt");

    /*
    auto picture = tgui::Picture::create( "Data\\img\\Screens\\main.jpg" );
    picture->setSize("100% + 1", "100% + 1");
    gui->add(picture);
    */

    auto editBoxUsername = tgui::EditBox::create();
    editBoxUsername->setSize({300, 40});
    editBoxUsername->setPosition({"50% - 150", "50% - 25"});
    editBoxUsername->setDefaultText("Username");
    editBoxUsername->setTextSize(24);
    gui->add(editBoxUsername, "username");

    // Create the password edit box
    auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
    editBoxPassword->setPosition({"50% - 150", "50% + 25"});
    editBoxPassword->setPasswordCharacter('*');
    editBoxPassword->setDefaultText("Password");
    gui->add(editBoxPassword, "password");

    // Create the login button
    auto buttonLogin = tgui::Button::create("Login");
    buttonLogin->setSize({300, 40});
    buttonLogin->setPosition({"50% - 150", "50% + 75"});
    buttonLogin->setTextSize(24);
    gui->add(buttonLogin, "Login");

    auto buttonExit = tgui::Button::copy(buttonLogin);
    buttonExit->setText("Exit");
    buttonExit->setPosition( {"100% - 310", "100% - 50"} );
    gui->add(buttonExit, "Exit");

    // failed login button
    auto buttonFailed = tgui::Button::copy(buttonLogin);
    buttonFailed->setSize({600, 80});
    buttonFailed->setText("World server is down.");
    buttonFailed->setPosition( {"50% - 300", "50%"} );
    buttonFailed->setVisible(false);
    gui->add(buttonFailed, "Failed");

    //gui->saveWidgetsToFile("widget.txt");
}

void Intro::onCreate()
{
    m_timePassed = 0.0f;
    EventManager* evMgr = m_stateMgr->getContext()->m_eventManager;
    //tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    evMgr->addCallback(StateType::Intro, "Esc", &Intro::exit, this);
}

void Intro::onDestroy()
{
    EventManager* evMgr = m_stateMgr->getContext()->m_eventManager;

    evMgr->removeCallback(StateType::Intro,"Ent");
    evMgr->removeCallback(StateType::Intro, "Esc");
}

void Intro::update(float & dTime)
{
    m_timePassed += dTime;
}

void Intro::draw()
{
    //sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
}

void Intro::exit(EventDetails* details)
{
    m_stateMgr->getContext()->m_wind->close(nullptr);
}

void Intro::login(EventDetails* details)
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    std::string username = gui->get<tgui::EditBox>("username")->getText();
    std::string password = gui->get<tgui::EditBox>("password")->getText();

    /// \todo
        if(username == "") username = "qwe";
        if(password == "") password = "qwe";

    sf::Uint8 result= m_stateMgr->getContext()->m_client->connect(username, password);
    if( result == 0 )
    {
        m_stateMgr->switchTo(StateType::CharacterScreen);
        return;
    }

    auto w = gui->get<tgui::Button>("Failed");
    if(result == 3)
    {
        w->setText("World server is down.");
    }
    else if(result == 4)
    {
        w->setText("Invalid username or password.");
    }
    else if(result == 5)
    {
        w->setText("Invalid username or password.");
    }
    else
    {
        w->setText("Cannot connect to server\nCheck your internet connection.");
    }

    for(auto w: gui->getWidgets() )
        w->setEnabled(false);

    w->setVisible(true);
    w->setEnabled(true);
}

void Intro::infoBox()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    auto widget = gui->get("Failed");
    if(widget)
        widget->setVisible(false);

    for(auto w: gui->getWidgets() )
        w->setEnabled(true);
}

void Intro::activate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    loadGUI();

    //login button
    auto widget = gui->get("Login");
    if(widget)
        widget->connect("pressed", &login, this, nullptr);

    //exit
    widget = gui->get("Exit");
    if(widget)
        widget->connect("pressed", &exit, this, nullptr);

    widget = gui->get("Failed");
    if(widget)
        widget->connect("pressed", &infoBox, this);

}

void Intro::deactivate()
{
    tgui::Gui * gui = m_stateMgr->getContext()->m_gui;

    gui->removeAllWidgets();
}
