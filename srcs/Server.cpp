#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(std::string pword, std::string given_port) : password(pword), port(given_port)
{
}

Server::Server( const Server & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


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
	add_socket_to_list(&this->pfds, listener, POLLIN, 0);
}

void	Server::add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	pfds->push_back(tmp);
}

void	Server::poll_loop()
{
	while (1)
	{
		std::cout << "polling fds..." << std::endl;
		polling();
		handle_pfds();
	}
}

void	Server::polling()
{
	this->arr_pfds = (pollfd *)malloc(sizeof(this->arr_pfds) * sizeof(this->pfds.size()));
	std::copy(this->pfds.begin(), this->pfds.end(), this->arr_pfds);
	poll(this->arr_pfds, this->pfds.size(), -1);
	std::copy(this->arr_pfds, this->arr_pfds + this->pfds.size(), this->pfds.begin());
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
				this->handle_command(it);
		}
	}
}

void	Server::handle_new_connection()
{
	struct sockaddr_storage	remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->listener, (struct sockaddr *)&remote_addr, &addr_size);

	/* get CAP */
/*	std::cout << "cap part" << std::endl;
	recv(new_fd, this->buf, sizeof(char[510]), 0);
	std::string cap(buf);
	cap = cap.substr(cap.find(" ") + 1, cap.find("\n"));
	std::cout << "cap :" << cap << "#\n" << std::endl;

	/* get PASS */
/*	std::cout << "pass part" << std::endl;
	recv(new_fd, this->buf, sizeof(char[510]), 0);
	std::string pass(buf);
	pass = pass.substr(pass.find(" ") + 1, pass.length() - 7);
	std::cout << "pass :" << pass << "#\n" << std::endl;

	/* get NICK */
/*	std::cout << "nick part" << std::endl;
	recv(new_fd, this->buf, sizeof(char[510]), 0);
	std::string nick(buf);
	nick = nick.substr(nick.find(" ") + 1, nick.length() - 7);
	std::cout << "nick :" << nick << "#\n" << std::endl;
*/
	std::cout << "wtf" << std::endl;
	while (this->new_connection.get_cap().empty() || this->new_connection.get_nick().empty() || this->new_connection.get_pass().empty())
	{
		std::string tmp;
		if (tmp.empty())
		{
			recv(new_fd, this->buf, sizeof(char[510]), 0);
			tmp = this->buf;
		}
		tmp = this->new_connection.handle_new_entry(tmp);
	}

	std::cout << "cap :" << this->new_connection.get_cap() << std::endl;
	std::cout << "nick :" << this->new_connection.get_nick() << std::endl;
	std::cout << "pass :" << this->new_connection.get_pass() << std::endl;

	//if ((pass.substr(5, pass.length() - 7)).compare(this->password) == 0)
	//{
		add_socket_to_list(&this->pfds, new_fd, POLLIN, 0);
		std::string reply = ":127.0.0.1 001 " + this->new_connection.get_nick() + " :Welcome to the Internet Relay Network 127.0.0.1";
		std::cout << send(new_fd, reply.c_str(), 510, SOCK_STREAM) << std::endl;
		std::cout << reply << std::endl;
		std::cout << "pollserver: new connection" << std::endl;
	//}
	//else
//	{
//		std::cout << "error" << std::endl;
//	}

}

void	Server::handle_command(std::list<pollfd>::iterator it)
{
	int	nbytes = recv(it->fd, this->buf, sizeof(char[510]), 0);
	std::cout << this->buf << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Server::get_password() { return ( this->password ); }

/* ************************************************************************** */