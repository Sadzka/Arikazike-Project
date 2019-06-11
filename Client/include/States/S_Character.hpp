#pragma once
#include <TGUI/Gui.hpp>
#include "States/State.hpp"
#include "Input/EventDetails.hpp"
#include "Input/EventManager.hpp"
#include "States/StateManager.hpp"

class S_Character : public State
{
    void loadGUI();
    sf::CircleShape m_circleSelect;
    int m_select;
    sf::Uint64 m_cid;

public:
    S_Character(StateManager* stateManager);
    ~S_Character();

    virtual void onCreate();
    virtual void onDestroy();

    virtual void activate();
    virtual void deactivate();

    virtual void update(float & dTime);
    virtual void draw();

    void selectCharacter(const int & select, const sf::Uint64 & cid);
    void enterWorld();

};
