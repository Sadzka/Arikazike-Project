#include <iostream>
#include <sstream>
#include <windows.h>
#include "PacketHandler.hpp"
#include "Server.hpp"
#include "Console/Commands.hpp"

void CommandProcess(Server* server)
{
    CommandsHandler cmdHnd(server);
	while (server->isRunning())
    {
		std::string str;
		std::getline(std::cin, str);
		std::stringstream stream(str);

		cmdHnd.handleCommand(stream);

        Sleep(1000);
	}
}

int main()
{
	Server server(Handler);

	if (server.start())
    {
        server.connectMySQL();
		sf::Thread c(&CommandProcess, &server);
		c.launch();

		sf::Clock clock;
		clock.restart();
		while (server.isRunning())
        {
			server.update(clock.restart());
			Sleep(33);
		}
		std::cout << "Stopping server... \n";
	}

	system("PAUSE");
	return 0;

}
