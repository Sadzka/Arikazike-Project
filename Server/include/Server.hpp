#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <iostream>
#include "ClientInfo.hpp"
#include "../../Shared/include/PacketTypes.hpp"
#include "Database/MySQL.hpp"
#include "PacketHandler.hpp"
#include "DestructibleInfo.hpp"

class Server;

using Clients = std::unordered_map<sf::Uint16, ClientInfo>;
using Destructible = std::unordered_map<sf::Uint32, DestructibleInfo>;
using PacketHandler = std::function<void(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*)>;
using TimeoutHandler = std::function<void(const sf::Uint16&)>;

class Server
{
    int cno;
    int irno;
    int did;

    friend class CommandsHandler;
    friend void Handler(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*);

	sf::UdpSocket m_incoming;
	sf::UdpSocket m_outgoing;

	PacketHandler m_packetHandler;
	//TimeoutHandler m_timeoutHandler;

	Clients m_clients;
	Destructible m_destructible;

	const float POSITION_UPDATE_INTERVAL;
	float m_positionUpdateTime;
	sf::Time m_serverTime;

	bool m_running;

	sf::Thread m_listenThread;
	sf::Mutex m_mutex;

	size_t m_totalSent;
	size_t m_totalReceived;

    void setup();

public:

    MySQL mysql;

    template <class T>
    Server(void(T::*handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*), T* instance): m_listenThread(&Server::listen, this)
    {
        m_packetHandler = std::bind(handler, instance,
                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                          std::placeholders::_4, std::placeholders::_5);
    }

    Server(void(*handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*));
    ~Server();

    /*
    template<class T>
    void bindTimeoutHandler(void(T::*handler)(const ClientID&), T* instance)
    {
        m_timeoutHandler = std::bind(handler, instance, std::placeholders::_1);
    }
    */

    bool start();
    bool stop();
    void disconnectAll();
    void connectMySQL();

    void bindTimeoutHandler(void(*handler)(const ClientID&));
    bool send(const ClientID& id, sf::Packet& packet);
    bool send(sf::IpAddress& ip, const PortNumber& port, sf::Packet& packet);
    void broadcast(sf::Packet& packet, const ClientID& ignore = ClientID(Network::NullID));

    void listen();
    void update(const sf::Time& time);
    void updatePositions();

    ClientID addClient(const sf::IpAddress& ip, const PortNumber& port, const sf::Uint16 & cid, const std::string & username = "Unknown");
    ClientID addClient(const ClientInfo & client);
    void removeClient(auto & itr);
    bool removeClient(const ClientID& id);
    bool removeClient(const sf::IpAddress& ip, const PortNumber& port);

    bool hasClient(const ClientID& id);
    bool hasClient(const sf::IpAddress& ip, const PortNumber& port);

    void sendRemoveDestructible(const int &type, const int &id);

    bool isRunning();

    ClientInfo * findClient(const ClientID& id);
    Entity * findEntity(const unsigned & id);
    bool getClientInfo(const ClientID& id, ClientInfo& info);
    ClientID getClientID(const sf::IpAddress& ip, const PortNumber& port);
    unsigned int getClientCount();
    std::string getClientList();
    sf::Mutex& getMutex();
    sf::Time getServerTime()const;

    void removeCharacter(const ClientID& id);
    Clients getClients();
};

