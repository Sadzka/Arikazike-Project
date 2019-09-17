#include "States/StateManager.hpp"

StateManager::StateManager(Shared* shared) : m_shared(shared)
{
    registerState<Intro>(StateType::Intro);
    registerState<S_Character>(StateType::CharacterScreen);
    registerState<S_Game>(StateType::Game);
}

StateManager::~StateManager()
{
    for (auto &itr : m_states)
    {
        itr.second->onDestroy();
        delete itr.second;
    }
}

void StateManager::draw()
{
    if (m_states.empty())
    {
        return;
    }
    if (m_states.back().second->isTransparent()
    &&  m_states.size() > 1)
    {
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end())
            {
                if (!itr->second->isTransparent())
                {
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); itr++)
        {
            m_shared->m_wind->getRenderWindow()->setView(itr->second->getView());
            itr->second->draw();
        }
    }
    else
    {
        m_states.back().second->draw();
    }
}

void StateManager::update(float & dTime)
{
    if (m_states.empty()){ return; }
    if (m_states.back().second->isTranscendent()
    && m_states.size() > 1)
    {
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end())
                if (!itr->second->isTranscendent())
                    break;
            --itr;
        }
        for (; itr != m_states.end(); ++itr)
            itr->second->update( dTime );

    }
    else
        m_states.back().second->update( dTime );

}

Shared* StateManager::getContext() { return m_shared; }

bool StateManager::hasState(const StateType& type)
{
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            auto removed = std::find(m_toRemove.begin(),
            m_toRemove.end(), type);
            if (removed == m_toRemove.end())
                return true;
            return false;
        }
    }
    return false;
}

void StateManager::remove(const StateType& type) { m_toRemove.push_back(type); }

void StateManager::processRequests()
{
    while (m_toRemove.begin() != m_toRemove.end())
    {
        removeState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}

void StateManager::switchTo(const StateType& type)
{
    m_shared->m_eventManager->setCurrentState(type);
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            m_states.back().second->deactivate();
            StateType tmp_type = itr->first;
            State* tmp_state = itr->second;
            m_states.erase(itr);
            m_states.emplace_back(tmp_type, tmp_state);
            tmp_state->activate();
            m_shared->m_wind->getRenderWindow()->setView(tmp_state->getView());
            return;
        }
    }
    // State with type wasn't found.

    if (!m_states.empty())
        m_states.back().second->deactivate();
    createState(type);
    m_states.back().second->activate();
    m_shared->m_wind->getRenderWindow()->setView(m_states.back().second->getView());
}

void StateManager::createState(const StateType& type)
{
    auto newState = m_stateFactory.find(type);
    if (newState == m_stateFactory.end()){ return; }
    State* state = newState->second();
    state->m_view = m_shared->m_wind->getRenderWindow()->getDefaultView();

    m_states.emplace_back(type, state);
    state->onCreate();
}

void StateManager::removeState(const StateType& type)
{
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            itr->second->onDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}
