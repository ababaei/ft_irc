#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <string>
# include <cstring>
# include <list>
# include <unistd.h>
# include <fcntl.h>

#define PORT "6667"

class Server
{

	public:

		Server(std::string pword, std::string arg_port);
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

		void			set_listener_sock(); // set le socket listener
		void			poll_loop();
		void			polling();
		void			handle_pfds();
		void			handle_new_connection();
		void			handle_command(std::list<pollfd>::iterator it);
		void			add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev);
		
		/* accessors */
		std::string		get_password();


	private:

		std::string			password;
		std::string			port;
		std::list<pollfd>	pfds;
		struct pollfd		*arr_pfds;
		char				buf[510];
		int					listener;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */