#include "../inc/Server.hpp"
#include <cstring>
#include <cstdlib>
#include <cctype>

#define PORT "6667"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Not enough argument: <port> <password>\n";
		return (1);
	}
	for (int i = 0; i < strlen(av[1]); i++)
	{
		if (isdigit(av[1][i]) == 0)
		{
			std::cerr << "Wrong port argument\n";
			return (1);
		}
	}
	
    Server  irc_serv(av[1], av[2]);

    // std::cout << irc_serv.get_password() << std::endl;
    // std::cout << "get pwd : " << irc_serv.get_password() << std::endl;
    irc_serv.set_listener_sock();
    irc_serv.poll_loop();
	return (0);
}