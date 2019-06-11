#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "Server.hpp"

class CommandsHandler
{
    Server * m_server;
    void toUpper(std::string & str, int start = 0, int end = -1 );
    void toLower(std::string & str, int start = 0, int end = -1 );
    void fixName(std::string & name);
    bool areYouSure();
public:

    CommandsHandler(Server * server);
    ~CommandsHandler();

    void handleCommand(std::stringstream & stream);

};
