#include "Console/Commands.hpp"

using std::cout;
using std::endl;

CommandsHandler::CommandsHandler(Server * server) : m_server(server) {}
CommandsHandler::~CommandsHandler() { m_server = nullptr; }

void CommandsHandler::toUpper(std::string & str, int start, int end)
{
    if(end == -1)
        end = str.length();

    for (int i = start; i < end; i++ )
    {
        str[i] = toupper(str[i]);
    }

}
void CommandsHandler::toLower(std::string & str, int start, int end)
{
    if(end == -1)
        end = str.length();

    for (int i = start; i < end; i++ )
    {
        str[i] = tolower(str[i]);
    }

}

bool CommandsHandler::areYouSure()
{
    char res;
    std::cout<<"Are you sure? (y/n): ";
    std::cin >> res;
    if(res == 'Y' || res == 'y')
        return true;
    else return false;
}

void CommandsHandler::fixName(std::string & name)
{
    toUpper(name, 0, 1);
    toLower(name, 1);
}

void CommandsHandler::handleCommand(std::stringstream & stream)
{
    std::string keyword;
    stream >> keyword;
    toUpper(keyword);

    if(keyword == "HELP")
    {
        std::string arg;
        if( stream >> arg)
        {
            toUpper(arg);

            if(arg == "MESSAGE")
            {
                cout << "    Send message to all users." << endl << endl;
                cout << "    Options: " << endl;
                cout << "      -A <...> <msg>     Send message to all users." << endl;
                cout << "      -P <...> <msg>     Send to player with name" << endl;
                cout << endl;
                return;
            }
            if(arg == "KICK")
            {
                cout << "    Kick a player." << endl << endl;
                cout << "    Options: " << endl;
                cout << "      -N <...> <msg>      Kick by name" << endl;
                cout << "      -I <...> <msg>      Kick by ID" << endl;
                cout << "      -A <msg>            Kick by all users" << endl;
                cout << endl;
                return;
            }
            if(arg == "SHUTDOWN")
            {
                cout << "    Quit the server." << endl << endl;
                cout << "    Options: " << endl;
                cout << "      -T <...> <msg>      Time to quit." << endl;
                cout << "      -C                  Cancel quitting." << endl;
                cout << endl;
                return;
            }
            if(arg == "LIST")
            {
                cout << "TODO." << endl;
                return;
            }

            cout << "This command is not supported by the help utility." << endl;
            return;
        }
        cout << "For more information on a specific command, type HELP <command-name>" << endl;
        cout << "MESSAGE        Send message to all users." << endl;
        cout << "KICK           Kick a player." << endl;
        cout << "QUIT           Quit the server." << endl;
        cout << endl;
        return;
    }
    else if( keyword == "LIST")
    {
        std::string arg;
        std::string options;
        int param = 0;
        bool paramters[7] = {false};
        while( stream >> options)
        {
            if(options == "-U") paramters[0] = true;
            else if(options == "-UID") paramters[1] = true;
            else if(options == "-IP") paramters[2] = true;
            else if(options == "-C") paramters[3] = true;
            else if(options == "-CID") paramters[4] = true;
            else if(options == "-L") paramters[5] = true;
            else if(options == "-P") paramters[6] = true;
            param++;
        }

        if(param == 0)
        {
            for (int i = 0; i < 7; i++ )
            {
                paramters[i] = true;
            }
        }
        cout << "| ";
        if( paramters[0] ) cout << "Username \t| ";
        if( paramters[1] ) cout << "User ID  \t| ";
        if( paramters[2] ) cout << "IP       \t| ";
        if( paramters[3] ) cout << "Character\t| ";
        if( paramters[4] ) cout << "Char ID  \t| ";
        if( paramters[5] ) cout << "Location \t| ";
        if( paramters[6] ) cout << "Position \t| ";

        cout << "\n";

        for (auto itr : m_server->m_clients )
        {
            cout << "| ";
            if( paramters[0] ) cout << itr.second.m_username << "\t\t| ";
            if( paramters[1] ) cout << itr.second.m_clientID << "\t\t| ";
            if( paramters[2] ) cout << itr.second.m_clientIP << "\t| ";

            if(itr.second.m_character != nullptr)
            {
                if( paramters[3] ) cout << itr.second.m_character->getName() << "\t\t| ";
                if( paramters[4] ) cout << itr.second.m_character->getId() << "\t\t| ";
                if( paramters[5] ) cout << itr.second.m_character->getLocation() << "\t\t| ";
                if( paramters[6] ) cout << "(" << (int)itr.second.m_character->getPosition().x << ", " << (int)itr.second.m_character->getPosition().y << ")\t|";
            }
            else
            {
                for (int i = 3; i < 7; i++ )
                    if( paramters[i] ) cout << " {null} \t| ";
            }
            cout << endl;
        }
        cout << "\n\nTotal clients: " << m_server->m_clients.size() << endl;
    }
    else if( keyword == "MESSAGE")
    {
        std::string options;
        if( stream >> options)
        {
            toUpper(options);
            if(options == "-A")
            {
                std::string message;
                getline(stream, message);
                cout << "SERVER MESSAGE:" << message << endl;
                /// \todo send message
                cout << endl;
                return;
            }
            if(options == "-P")
            {
                std::string player;
                stream >> player;
                fixName(player);

                std::string message;
                getline(stream, message);

                cout << "To " << player << ":" << message << endl;
                /// \todo send message
                cout << endl;
                return;
            }
        }


        cout << "Not enought arguments, type 'help' for more informations." << endl;
        cout << endl;
        return;
    }

    else if( keyword == "KICK")
    {
        std::string options;
        if( stream >> options)
        {
            toUpper(options);
            if(options == "-N")
            {
                std::string name;
                stream >> name;
                if(name == "")
                {
                    cout << "Not enought arguments, type 'help' for more informations." << endl;
                    cout << endl;
                    return;
                }
                fixName(name);

                std::string reason;
                getline(stream, reason);
                if(reason == "") reason = " no reason";

                cout << "Kicked " << name << " for reason:" << reason << endl;
                /// \todo send message
                cout << endl;
                return;
            }
            if(options == "-I")
            {
                int id;
                if(!(stream >> id) )
                {
                    cout << "Not enought arguments, type 'help' for more informations." << endl;
                    cout << endl;
                    return;
                }

                std::string reason;
                getline(stream, reason);
                if(reason == "") reason = " no reason";

                cout << "Kicked player with ID: " << id << " for reason:" << reason << endl;
                /// \todo send message
                cout << endl;
                return;
            }
            if(options == "-A")
            {
                if(!areYouSure()) return;
                std::string reason;
                getline(stream, reason);
                if(reason == "") reason = " no reason";

                cout << "Kicked all players" << " for reason:" << reason << endl;
                /// \todo send message
                cout << endl;
                return;
            }

        }
        cout << "Not enought arguments, type 'help' for more informations." << endl;
        cout << endl;
        return;
    }

    else if( keyword == "QUIT")
    {
        std::string options;
        std::string message;
        int time = 0;

        stream >> options;

        toUpper(options);

        if(options == "-T")
        {
            if( !(stream >> time) )
            {
                time = 0;
                cout << "Bad arguments, type 'help' for more informations." << endl;
                cout << endl;
                return;
            }
        }

        if(options == "-C")
        {
            cout << "Canceling shut downing." << endl;
            cout << endl;
            return;
        }


        std::string reason;
        getline(stream, reason);

        if(areYouSure())
        {
            if(time == 0)
            {
                m_server->stop();
                cout << "The Server had been shuted down." << endl;
            }
            else
                cout << "The Server will shutdown after: " << time << " seconds." << reason << endl;
            return;
        }
        /// \todo send message
        cout << "Canceled..." << endl;
        cout << endl;
        return;
    }

}
