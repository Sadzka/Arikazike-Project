#include "States/State.hpp"

void State::setTransparent(const bool& l_transparent) { m_transparent = l_transparent; }
bool State::isTransparent()const { return m_transparent; }
void State::setTranscendent(const bool& l_transcendence) { m_transcendent = l_transcendence; }
bool State::isTranscendent()const { return m_transcendent; }

sf::View & State::getView() { return m_view; }
StateManager* State::getStateManager() { return m_stateMgr; }
