#pragma once
#include <TGUI/Gui.hpp>
#include "States/State.hpp"
#include "Input/EventDetails.hpp"
#include "Input/EventManager.hpp"
#include "States/StateManager.hpp"

class Intro : public State
{
    float m_timePassed;

    void loadGUI();
    void exit(EventDetails* details);
    void login(EventDetails* details);
    void infoBox();

public:
    Intro(StateManager* stateManager);
    ~Intro();

    virtual void onCreate();
    virtual void onDestroy();

    virtual void activate();
    virtual void deactivate();

    virtual void update(float & dTime);
    virtual void draw();
};
