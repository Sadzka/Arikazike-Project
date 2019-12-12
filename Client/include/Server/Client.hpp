#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <functional>
#include <fstream>
#include "Shared.hpp"
#include "PacketTypes.hpp"
#include "NetworkSettings.hpp"

#define CONNECT_TIMEOUT 5000 // Milliseconds.
using std::cout;

class Client;

/// \todo \\\ ////

using PacketHandler = std::function< void(const sf::Uint16&, sf::Packet&, Client*)>;

class Client
{
    std::vector <int> m_unknownEntities;

    std::string     m_playerName;
    sf::UdpSocket   m_socket;
    sf::IpAddress   m_serverIp;
    sf::Uint16      m_serverPort;
    PacketHandler   m_packetHandler;
    bool            m_connected;
    sf::Time        m_serverTime;
    sf::Time        m_lastHeartbeat;
    float           m_dTime;
    sf::Thread      m_listenThread;
    sf::Mutex       m_mutex;

    //float           m_inputLatencyTime;
    //const float     m_inputLatency;

    sf::Uint8       m_dx;   // is moving
    sf::Uint8       m_dy;   // is moving

    Shared *        m_shared;
    Entity *        m_player;

    bool            m_waitPacket;

    void handlePacket(const sf::Uint16& id, sf::Packet& packet, Client* client);
    void getServerIP();

public:
    Client(Shared * shared);
    ~Client();
    //bool isInputReady();
    sf::Uint8 connect(std::string username, std::string password);
    bool disconnect();
    void listen();
    bool send(sf::Packet& packet);

    template<class T>
    void setup(void(T::*handler) (const sf::Uint16&, sf::Packet&, Client*), T* instance)
    {
        m_packetHandler = std::bind(handler, instance,
                          std::placeholders::_1, std::placeholders::_2,
                          std::placeholders::_3);
    }

    void setup(void(*handler)(const sf::Uint16&, sf::Packet&, Client*));
    void unregisterPacketHandler();
    void update(const float & time);
    bool isConnected() const;

    void setPlayerName(const std::string& name);
    void setTime(const sf::Time& time);
    void setServerInformation(const sf::IpAddress& ip, const sf::Uint16& port);

    const sf::Time & getTime() const;
    const sf::Time & getLastHeartbeat() const;
    Entity * getPlayer();
    sf::Mutex & getMutex();

    void addUnknownEntity(const int & id);
    bool isWaiting();
    void getCharacterList();
    void enterWorld(const sf::Uint64 & cid);
    void move(const int & x, const int & y);
    void getCharacterDetails();
};

void stampPacket(const PacketType& type, sf::Packet& packet);
