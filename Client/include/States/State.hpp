#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
class StateManager;

class State
{
    friend class StateManager;

protected:
    StateManager* m_stateMgr;
    bool m_transparent;
    bool m_transcendent;
    sf::View m_view;

public:
    State(StateManager* stateManager) : m_stateMgr(stateManager), m_transparent(false), m_transcendent(false) {}
    virtual ~State() {}
    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void update(float & dTime) = 0;
    virtual void draw() = 0;

    void setTransparent(const bool& l_transparent);
    void setTranscendent(const bool& l_transcendence);

    bool isTransparent()const;
    bool isTranscendent()const;

    StateManager* getStateManager();
    sf::View & getView();
};
