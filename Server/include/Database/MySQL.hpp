#pragma once
#include <windows.h>
#include <mysql.h>
#include <iostream>
#include "../../Shared/include/Entity/Entity.hpp"
#include "../../Shared/include/Entity/Components/Component.hpp"
#include "../../Shared/include/Entity/Components/Health.hpp"
#include "../../Shared/include/Entity/Components/CMana.hpp"
#include "Utilities/sha512.h"
#include "ClientInfo.hpp"
class Server;

class MySQL
{
    const char to_replace[7] = { '-', '\"', '\'', '`', '*', '=', ' ' };
    const char to_replaceSize = 7;
    MYSQL * connection;
    std::string ask;
    MYSQL_ROW row;
    MYSQL_RES result;
    Server * m_server;

    void HTMLEntities(std::string & str);
public:
    MySQL(Server * server);
    ~MySQL();

    /// Handle PacketType::Connect
    /// \param username and password
    /// \return bool, true if succes.
    int  _Connect(std::string & username, std::string & password);
    bool _GetCharacterList(const sf::Uint32 & id, sf::Packet & packet);
    void _GetCharacter(ClientInfo * client, sf::Uint32 characterID, sf::Packet & packet);


    bool connect();
};
