#include "Input/EventManager.hpp"

EventManager::EventManager(): m_hasFocus(true) { loadBindings(); }
EventManager::~EventManager()
{
    for (auto &itr : m_bindings)
    {
        delete itr.second;
        itr.second = nullptr;
    }
}

bool EventManager::addBinding(Binding *binding)
{
    if (m_bindings.find(binding->m_name) != m_bindings.end())
        return false;
    return m_bindings.emplace(binding->m_name, binding).second;
}

bool EventManager::removeBinding(std::string name)
{
    auto itr = m_bindings.find(name);
    if (itr == m_bindings.end()){ return false; }
    delete itr->second;
    m_bindings.erase(itr);
    return true;
}

void EventManager::setFocus(const bool& focus) { m_hasFocus = focus; }

void EventManager::handleEvent(sf::Event& event)
{
    // Handling SFML events.
    for (auto &b_itr : m_bindings)
    {
        Binding* bind = b_itr.second;
        for (auto &e_itr : bind->m_events)
        {
            EventType sfmlEvent = (EventType)event.type;
            if (e_itr.first != sfmlEvent) { continue; }
            if (sfmlEvent == EventType::KeyDown
            ||  sfmlEvent == EventType::KeyUp)
            {
                if (e_itr.second.m_code == event.key.code)
                {
                    // Matching event/keystroke.
                    // Increase count.
                    if (bind->m_details.m_keyCode != -1)
                        bind->m_details.m_keyCode = e_itr.second.m_code;

                    ++(bind->c);
                    break;
                }
            }
            else if (sfmlEvent == EventType::MButtonDown
            ||       sfmlEvent == EventType::MButtonUp)
            {
                if (e_itr.second.m_code == event.mouseButton.button)
                {
                    // Matching event/keystroke.
                    // Increase count.
                    bind->m_details.m_mouse.x = event.mouseButton.x;
                    bind->m_details.m_mouse.y = event.mouseButton.y;
                    if (bind->m_details.m_keyCode != -1)
                        bind->m_details.m_keyCode = e_itr.second.m_code;

                    ++(bind->c);
                    break;
                }
            }
            else
            {
                // No need for additional checking.
                if (sfmlEvent == EventType::MouseWheel)
                {
                    bind->m_details.m_mouseWheelDelta =
                    event.mouseWheel.delta;
                }
                else if (sfmlEvent == EventType::WindowResized)
                {
                    bind->m_details.m_size.x = event.size.width;
                    bind->m_details.m_size.y = event.size.height;
                }
                else if (sfmlEvent == EventType::TextEntered)
                    bind->m_details.m_textEntered = event.text.unicode;

                ++(bind->c);
            }
        }
    }
}

sf::Vector2i EventManager::getMousePos(sf::RenderWindow* wind)
{
    return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());
}

void EventManager::update()
{
    if (!m_hasFocus){ return; }
    for (auto &b_itr : m_bindings)
    {
        Binding* bind = b_itr.second;
        for (auto &e_itr : bind->m_events)
        {
            switch (e_itr.first)
            {
                case(EventType::Keyboard):
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.m_code)))
                    {
                        if (bind->m_details.m_keyCode != -1)
                            bind->m_details.m_keyCode = e_itr.second.m_code;

                        ++(bind->c);
                    }
                break;
                case(EventType::Mouse):
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.m_code)))
                    {
                        if (bind->m_details.m_keyCode != -1)
                            bind->m_details.m_keyCode = e_itr.second.m_code;

                        ++(bind->c);
                    }
                break;
            }
        }
        if (bind->m_events.size() == bind->c)
        {
            auto stateCallbacks = m_callbacks.find(m_currentState);
            auto otherCallbacks = m_callbacks.find(StateType(0));

            if (stateCallbacks != m_callbacks.end())
            {
                auto callItr = stateCallbacks->second.find(bind->m_name);
                // Pass in information about events.
                if (callItr != stateCallbacks->second.end())
                    callItr->second(&bind->m_details);

            }
            if (otherCallbacks != m_callbacks.end())
            {
                auto callItr = otherCallbacks->second.find(bind->m_name);
                // Pass in information about events.
                if (callItr != otherCallbacks->second.end())
                    callItr->second(&bind->m_details);

            }
        }
        bind->c = 0;
        bind->m_details.clear();
    }
}

void EventManager::loadBindings()
{
    std::string delimiter = ":";
    std::fstream bindings;
    bindings.open("keys.txt", std::ios::in);
    if (!bindings.is_open())
    {
        std::cout << "! Failed loading 'keys.txt' ." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(bindings, line))
    {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        Binding* bind = new Binding(callbackName);
        while (!keystream.eof())
        {
            std::string keyval;
            keystream >> keyval;
            int start = 0;
            int end = keyval.find(delimiter);
            if (end == std::string::npos)
            {
                delete bind;
                bind = nullptr;
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end - start)));
            int code = stoi(keyval.substr(end + delimiter.length(),
            keyval.find(delimiter, end + delimiter.length())));
            EventInfo eventInfo;
            eventInfo.m_code = code;
            bind->BindEvent(type, eventInfo);
        }
        if (!addBinding(bind))
            delete bind;
        bind = nullptr;
    }
    bindings.close();
}

void EventManager::setCurrentState(StateType state) { m_currentState = state; }
