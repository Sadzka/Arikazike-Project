#pragma once
#include <SFML/Network.hpp>

using PacketID = sf::Uint16;

enum class PacketType
{
    Disconnect = 0,
    Connect = 1,
    Heartbeat = 2,
    GetCharacterList = 3,
    Message = 4,
    EnterWorld = 5,
    Move = 6,
    GetCharacterDetails = 7,
    OutOfBounds //always last
};

