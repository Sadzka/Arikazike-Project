#pragma once
#include <TGUI/Gui.hpp>
#include "States/State.hpp"
#include "Input/EventDetails.hpp"
#include "Input/EventManager.hpp"
#include "States/StateManager.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Directions.hpp"


using std::cout;
using std::endl;

class S_Game : public State
{
    void loadGUI();
    void exit(EventDetails* details);

public:
    S_Game(StateManager* stateManager);
    ~S_Game();

    virtual void onCreate();
    virtual void onDestroy();

    virtual void activate();
    virtual void deactivate();

    virtual void update(float & dTime);
    virtual void draw();

    void move(EventDetails* details);
};
