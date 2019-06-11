#pragma once
#include <SFML/Network.hpp>
#include "NetworkSettings.hpp"
#include "Entity/Entity.hpp"

struct ClientInfo
{
    friend class Server;

    sf::Uint16          m_clientID;
    std::string         m_username;
    sf::IpAddress       m_clientIP;
    sf::Uint16          m_clientPORT;
    sf::Time            m_lastHeartbeat;
    sf::Time            m_heartbeatSent;
    bool                m_heartbeatWaiting;
    unsigned short      m_heartbeatRetry;
    unsigned int        m_ping;
    Entity            * m_character;

    ClientInfo() {}
    ClientInfo(const sf::IpAddress& ip, const sf::Uint16 & port, const sf::Time& heartbeat,const std::string & username, sf::Uint32 id)
    : m_clientIP(ip), m_clientPORT(port), m_lastHeartbeat(heartbeat), m_clientID(id),
    m_heartbeatWaiting(false), m_heartbeatRetry(0), m_ping(0), m_username(username), m_character(nullptr)
    {}

    ClientInfo& operator=(const ClientInfo& rhs)
    {
        m_clientIP			= rhs.m_clientIP;
        m_clientPORT		= rhs.m_clientPORT;
        m_lastHeartbeat		= rhs.m_lastHeartbeat;
        m_heartbeatSent		= rhs.m_heartbeatSent;
        m_heartbeatWaiting	= rhs.m_heartbeatWaiting;
        m_heartbeatRetry	= rhs.m_heartbeatRetry;
        m_ping				= rhs.m_ping;
        m_clientID          = rhs.m_clientID;
        return *this;
    }
};
