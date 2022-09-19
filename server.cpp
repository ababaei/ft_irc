#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <unistd.h>

#define PORT "6667"

int	get_listener_sock(void)
{
	int listener;
	int yes = 1;
	int status;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);
	}
	
	for (p = servinfo; p != NULL; p = p->ai_next) {
		listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (listener < 0)
		{
			std::cerr << "socket error" << std::endl;
			continue ;
		}
		std::cout << listener << "\n";
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if ((bind(listener, servinfo->ai_addr, servinfo->ai_addrlen)) < 0)
		{
			std::cerr << "bind error" << std::endl;
			close(listener);
			continue ;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		std::cerr << "Server: failed to bind" << std::endl;
		exit(1);
	}

	if (listen(listener, 5) == -1)
	{
		std::cerr << "listen" << std::endl;
		exit(1);
	}

	std::cout << listener << "\n";
	return (listener);
}

void back_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	pfds->push_back(tmp);
}

int main(int argc, char **argv)
{
	std::list<pollfd> pfds;

	int listener;
	int new_fd;
	socklen_t addr_size;
	struct sockaddr_storage remoteaddr;
	struct pollfd *arr;

	char *buf[254];

	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	listener = get_listener_sock();
	if (listener == -1)
	{
		std::cerr << "Error getting a listening sock" << std::endl;
		exit(1);
	}

	back_socket_to_list(&pfds, listener, POLLIN, 0);
	
	std::cout << "listener fd: " << pfds.front().fd << "\n";
	std::cout << "pfds size: " << pfds.size() << "\n";

	while (1)
	{
		arr = (pollfd*)malloc(sizeof(*arr) * sizeof(pfds.size()));
		std::copy(pfds.begin(), pfds.end(), arr);
		int poll_count = poll(arr, pfds.size(), -1);

		if (poll_count == -1)
		{
			std::cerr << "poll" << "\n";
			exit(1);
		}

		itend = pfds.end();

		for (it = pfds.begin();it != itend; it++)
		{
			std::cout << "test fd:" << it->fd << "\n";
			if (it->revents & POLLIN)
			{
				if (it->fd == listener)
				{
					addr_size = sizeof(remoteaddr);
					new_fd = accept(listener, (struct sockaddr *)&remoteaddr, &addr_size);

					if (new_fd == -1)
					{
						std::cerr << "accept\n" ;
					}
					else
					{
						back_socket_to_list(&pfds, new_fd, 0, 0);
						std::cout << "pollserver: new connection\n";
					}
				}
				else
				{
					int nbytes = recv(it->fd, buf, sizeof(buf), 0);
					int sender_fd = it->fd;

					if (nbytes <= 0)
					{
						if (nbytes == 0)
							std::cout << "pollserver: socket\n";
						else
							std::cerr << "recv\n";
						close(it->fd);
						pfds.erase(it);
					}
					else
					{
						for(std::list<pollfd>::iterator i = pfds.begin(); i != itend; it++)
						{
							int dest_fd = i->fd;

							if (dest_fd != listener && dest_fd != sender_fd)
							{
								if (send(dest_fd, buf, nbytes, 0) == -1)
									std::cerr << "send\n";
							}
						}
					}
				}
			}	
		}
		free(arr);
	}
	return (0);
}
