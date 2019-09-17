#pragma once
#include "ClientInfo.hpp"
#include "Utilities/sha512.h"
#include "PacketTypes.hpp"
#include "NetworkSettings.hpp"
#include "Entity/Entity.hpp"
#include "Server.hpp"

using std::cout;
using std::endl;

void Handler(sf::IpAddress& ip, const PortNumber& port, const PacketID& pid, sf::Packet& packet, Server* server);

void stampPacket(const PacketType& type, sf::Packet& packet);
