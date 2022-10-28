#include "../../inc/Server.hpp"

/*  the first one is used to send msg to multiple socket,
    the second only to one. DEBUG is defined in Server.hpp
*/

void    Server::to_send(const std::string msg, std::vector<int> sockfds)
{
    std::vector<int>::iterator it;

    for (it = sockfds.begin(); it != sockfds.end(); it++)
    {
       	if (send(*it, msg.c_str(), msg.length(), 0) == -1)
    		std::cerr << "error send\n";
	    #ifdef DEBUG
		    std::cout << ">" << msg << "\n";
	    #endif
    }
}

void    Server::to_send(const std::string msg, int sockfd)
{
	if (send(sockfd, msg.c_str(), msg.length(), 0) == -1)
		std::cerr << "error send\n";
	#ifdef DEBUG
		std::cout << ">" << msg << "\n";
	#endif
}