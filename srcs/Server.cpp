#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(std::string pword, std::string given_port) : password(pword), port(given_port)
{
	this->address = "127.0.0.1";
}

Server::Server( const Server & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() { }


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Server::set_listener_sock(void)
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
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		fcntl(listener, F_SETFL, O_NONBLOCK);

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

	this->listener = listener;
	add_socket_to_list(listener, POLLIN, 0);
}

void	Server::add_socket_to_list(int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	this->pfds.push_back(tmp);
}

void	Server::poll_loop()
{
	while (1)
	{
		//std::cout << "polling fds..." << std::endl;
		polling();
		handle_pfds();
	}
}

void	Server::list_to_arr()
{
	this->arr_pfds = (pollfd *)malloc(sizeof(this->arr_pfds) * sizeof(this->pfds.size()));
	std::copy(this->pfds.begin(), this->pfds.end(), this->arr_pfds);
}

void	Server::arr_to_list()
{
	std::copy(this->arr_pfds, this->arr_pfds + this->pfds.size(), this->pfds.begin());
}

void	Server::polling()
{
	list_to_arr();
	poll(this->arr_pfds, this->pfds.size(), -1);
	arr_to_list();
	free(this->arr_pfds);
}

void	Server::handle_pfds()
{
	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	itend = this->pfds.end();
	for (it = this->pfds.begin(); it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == this->listener)
				this->handle_new_connection();
			else
			{
				int	nbytes = recv(it->fd, this->buf, sizeof(this->buf), 0);
				int sender_fd = it->fd;

				std::cout << "received " << nbytes << " bytes\n";
				if (nbytes <= 0)
				{
					this->close_connection(sender_fd, nbytes);
					this->pfds.erase(it++);
				}
				else
				{
					this->handle_raw(sender_fd, nbytes);
					this->User_list[sender_fd]->to_command(this->message);
				}
				std::cout << this->message << "\n";
				std::cout << "------------------------------\n";
			}
		}
	}
}

void	Server::handle_new_connection()
{
	struct sockaddr_storage	remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->listener, (struct sockaddr *)&remote_addr, &addr_size);

	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	add_socket_to_list(new_fd, POLLIN | POLLOUT, 0);
	User_list[new_fd] = new User(new_fd);
	std::cout << "pollserver: new connection\n";
}

void Server::close_connection(int sender_fd, int nbytes)
{	
	if (nbytes == 0)
		std::cout << "socket " << sender_fd << " hang up\n";
	else
		std::cerr << "recv\n";
	close(sender_fd);
}

void	Server::handle_raw(int sender_fd, int nbytes)
{
/* BROADCAST FUNCTION TO BE PUT ELSEWHERE
	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	itend = this->pfds.end();
	for (it = this->pfds.begin(); it != itend; it++)
	{
		if (it->fd != this->listener && it->fd != sender_fd)
		{
			if (send(it->fd, this->buf, nbytes, 0) == -1)
				std::cerr << "send back\n";
		}
	}
*/
	std::string tmp(this->buf);
	std::size_t pos;

	pos = tmp.find("\r\n");
	if (pos != std::string::npos)
	{
		message.clear(); //this should change and parse the message user side
	}
	message.append(this->buf, pos);
	memset(this->buf, 0, 510);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Server::get_password() { return ( this->password ); }

/* ************************************************************************** */
