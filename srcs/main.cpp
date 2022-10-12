#include "../inc/Server.hpp"

#define PORT "6667"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Not enough argument: <port> <password>\n";
		return (1);
	}
    Server  irc_serv(av[2], av[1]);

    std::cout << irc_serv.get_password() << std::endl;
    irc_serv.set_listener_sock();
    irc_serv.poll_loop();
	return (0);
}
