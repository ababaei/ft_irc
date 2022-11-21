#include "Server.hpp"

/*  the first one is used to send msg to multiple socket,
    the second only to one. DEBUG is defined in Server.hpp
*/

void    Server::toSend(const std::string msg, std::vector<int> sockfds)
{
    std::vector<int>::iterator it;
	int nbytes = 0;

    for (it = sockfds.begin(); it != sockfds.end(); it++)
    {
       	if ((nbytes = send(*it, msg.c_str(), msg.length(), 0)) == -1)
    		std::cerr << "error send\n";
	    #ifdef DEBUG
		    std::cout << ">" << msg << "|\n";
			std::cout << "sent = " << nbytes << "\n";
			std::cout << "to fd nb " << *it << std::endl;
	    #endif
    }
}

void    Server::toSend(const std::string msg, int sockfd)
{
	int nbytes = 0;

	if ((nbytes = send(sockfd, msg.c_str(), msg.length(), 0)) == -1)
		std::cerr << "error send\n";
	#ifdef DEBUG
		std::cout << ">" << msg << "|\n";
		std::cout << "sent = " << nbytes << std::endl;
	#endif
}
