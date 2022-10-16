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
# include "Register.hpp"
# include "Client.hpp"

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
		void			close_connection(int sender_fd, int nbytes);
		void			handle_command(int sender_fd, int nbytes);
		void			add_socket_to_list(int filed, short ev, short rev);
		//void			del_from_pfds(int sender_fd);
		void			arr_to_list();
		void			list_to_arr();
		std::string		reply(std::string reply_code, std::string target,std::string msg);
		
		/* accessors */
		std::string		get_password();

	private:

		std::string			password;
		std::string			port;
		std::string			address;
		std::list<pollfd>	pfds;
		struct pollfd		*arr_pfds;
		char				buf[510];
		int					listener;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
