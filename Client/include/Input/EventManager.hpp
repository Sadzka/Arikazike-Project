#pragma once
#include <unordered_map>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include "EventType.hpp"
#include "EventInfo.hpp"
#include "Binding.hpp"
#include "Utilities/EnumHash.hpp"

using Bindings = std::unordered_map<std::string, Binding*>;
using CallbackContainer = std::unordered_map< std::string, std::function<void(EventDetails*)> >;
enum class StateType;
//using Callbacks = std::unordered_map<std::string, std::function<void(EventDetails*)> >;
using Callbacks = utils::unordered_map<StateType, CallbackContainer>;

class EventManager
{
    Bindings        m_bindings;
    Callbacks       m_callbacks;
    StateType       m_currentState;
    bool            m_hasFocus;

    void loadBindings();

public:
    EventManager();
    ~EventManager();
    bool addBinding(Binding *binding);
    bool removeBinding(std::string name);
    void setFocus(const bool& focus);

    template<class T>
    bool addCallback(StateType state, const std::string& name, void(T::*func)(EventDetails*), T* instance)
    {
        auto itr = m_callbacks.emplace(state, CallbackContainer()).first;
        auto temp = std::bind(func, instance, std::placeholders::_1);
        return itr->second.emplace(name, temp).second;
    }
    bool removeCallback(StateType state, const std::string& name)
    {
        auto itr = m_callbacks.find(state);
        if (itr == m_callbacks.end())
            return false;
        auto itr2 = itr->second.find(name);
        if (itr2 == itr->second.end())
            return false;
        itr->second.erase(name);
        return true;
    }
    void handleEvent(sf::Event& event);
    void update();
    sf::Vector2i getMousePos(sf::RenderWindow* wind = nullptr);

    void setCurrentState(StateType state);
};
