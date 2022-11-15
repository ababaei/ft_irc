#include "../inc/Server.hpp"

#define PORT "6667"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Not enough argument: <port> <password>\n";
		return (1);
	}
	for (unsigned int i = 0; i < strlen(av[1]); i++)
	{
		if (isdigit(av[1][i]) == 0)
		{
			std::cerr << "Wrong port argument\n";
			return (1);
		}
	}
	
    Server  irc_serv(av[1], av[2]);

    irc_serv.setListenerSock();
    irc_serv.pollLoop();
	return (0);
}
