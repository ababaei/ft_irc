#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR/DESTRUCTOR --------------------------------
*/

Server::Server(std::string given_port, std::string pword) : _port(given_port), _password(pword)
{
	this->_address = "127.0.0.1";
	this->_server_name = "OurSuperServer";
}

Server::Server(const Server &src)
{
}

Server::~Server() {}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*vvvvvvvvvvvvvvvvvvvv Socket listener creation and adding socket to the server's list vvvvvvvvvvvvvvv*/
void Server::set_listener_sock(void)
{
	int listener;
	int yes = 1;
	int status;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, this->_port.c_str(), &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (listener < 0)
		{
			std::cerr << "socket error" << std::endl;
			continue;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		fcntl(listener, F_SETFL, O_NONBLOCK);

		if ((bind(listener, servinfo->ai_addr, servinfo->ai_addrlen)) < 0)
		{
			std::cerr << "bind error" << std::endl;
			close(listener);
			continue;
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

	this->_listener = listener;
	add_socket_to_list(listener, POLLIN, 0);
}

void Server::add_socket_to_list(int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	this->_pfds.push_back(tmp);
}

void Server::add_channel(std::string new_channel, Channel * chan)
{
	this->_channels.insert(std::pair<std::string, Channel*>(new_channel, chan));
}
/*	poll() function uses array and i wanted to work with container
	so i made two function to go to one from another.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::poll_loop()
{
	while (1)
	{
		// std::cout << "polling fds..." << std::endl;
		polling();
		handle_pfds();
		check_activity();
	}
}

void Server::list_to_arr()
{
	this->_arr_pfds = (pollfd *)malloc(sizeof(this->_arr_pfds) * sizeof(this->_pfds.size()));
	std::copy(this->_pfds.begin(), this->_pfds.end(), this->_arr_pfds);
}

void Server::arr_to_list()
{
	std::copy(this->_arr_pfds, this->_arr_pfds + this->_pfds.size(), this->_pfds.begin());
}

void Server::polling()
{
	list_to_arr();
	poll(this->_arr_pfds, this->_pfds.size(), -1);
	arr_to_list();
	free(this->_arr_pfds);
}

void Server::check_activity(void)
{
	std::map<int, User *>::iterator it;
	std::map<int, User *>::iterator itend;

	itend = this->_User_list.end();
	for (it = this->_User_list.begin(); it != itend; it++)
	{
		if (difftime(time(NULL), it->second->get_activity()) > 180)
		{
			// std::cout << "INACTIVE!\n";
			it->second->set_status("inactive");
		}
	}
}

bool	Server::isHere(const std::string& nick)
{
	for (std::map<int, User*>::iterator it = _User_list.begin(); it != _User_list.end();
			it++)
	{
		if (it->second->get_nick() == nick)
			return true;
	}
	return false;
}

/*  This function will check the results of poll() by doing a bitwise AND on the returned event.
	If its the listener that has something to say, that means we have a new connection.
	Otherwise, we have data to read with recv().
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::handle_pfds()
{
	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	itend = this->_pfds.end();
	for (it = this->_pfds.begin(); it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == this->_listener)
				this->handle_new_connection();
			else
			{
				int nbytes = recv(it->fd, this->_buf, sizeof(this->_buf), 0);
				int sender_fd = it->fd;
				std::cout << "< received " << nbytes << " bytes (" << this->_buf << ")\n";
				if (nbytes <= 0)
				{
					this->close_connection(sender_fd, nbytes);
					this->_pfds.erase(it++);
				}
				else
				{
					this->handle_raw(sender_fd, nbytes);
				}
				// std::cout << this->message << "\n";
				std::cout << "------------------------------\n";
			}
		}
	}
}

/*	This function will accept a new connection, add it to the server's fd list,
	alloc a new User and add it to the server's User map.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::handle_new_connection()
{
	struct sockaddr_storage remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->_listener, (struct sockaddr *)&remote_addr, &addr_size);

	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	add_socket_to_list(new_fd, POLLIN | POLLOUT, 0);
	_User_list[new_fd] = new User(new_fd, this);
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

void Server::handle_raw(int sender_fd, int nbytes)
{
	// vvvvvvvvvvvvvvvvvvvvvvv LOOP NEED TO BE CHECKED FOR SOME LOSSES CASES vvvvvvvvvvvvvvvvvvvvvvv
	std::string tmp(this->_buf);
	std::size_t pos;

	pos = tmp.find("\r\n");
	//std::cout << "FOUND!!__" << pos << "\n";
	while (((pos = tmp.find("\r\n")) != std::string::npos))
	{
		this->_User_list[sender_fd]->to_command(tmp.substr(0, pos));
		tmp = tmp.substr(pos + 2);
		_message.clear();
	}
	_message.append(this->_buf);
	memset(this->_buf, 0, 510);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string				Server::get_address(void) { return (this->_address); }

std::string				Server::get_server_name(void) { return (this->_server_name); }

std::string				Server::get_password() { return (this->_password); }

std::map<int, User *>	Server::get_user_list() { return (this->_User_list); }
std::map<std::string, Channel*> Server::get_channel_list() { return (this->_channels); }

Channel*				Server::get_channel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

User*					Server::get_user(const std::string& nick)
{
	for (std::map<int, User*>::iterator it = _User_list.begin(); it != _User_list.end();
			it++)
	{
		if (it->second->get_nick() == nick)
			return it->second;
	}
	return NULL;
}

void	Server::deleteChannel(const std::string& name)
{
	_channels.erase(name);
}
/* ************************************************************************** */