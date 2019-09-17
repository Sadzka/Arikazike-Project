#pragma once
#include "EventType.hpp"
#include "EventDetails.hpp"
using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding
{
    Events          m_events;
    std::string     m_name;
    EventDetails    m_details;
    int             c; // Count of events that are "happening".
    Binding(const std::string& name) : m_name(name), m_details(name), c(0) {}
    void BindEvent(EventType type, EventInfo info = EventInfo())
    {
        m_events.emplace_back(type, info);
    }
};
