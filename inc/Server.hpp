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
# include <new>
# include <map>
# include <unistd.h>
# include <ctime>
# include <fcntl.h>
# include "User.hpp"

#define PORT "6667"

class User;

class Server
{

	public:

		Server(std::string arg_port, std::string pword);
		Server( Server const & src );
		~Server(void);

		Server &		operator=( Server const & rhs );

		void			set_listener_sock(void); // set le socket listener
		void			add_socket_to_list(int filed, short ev, short rev);
		void			handle_pfds(void);
		void			handle_new_connection(void);
		void			close_connection(int sender_fd, int nbytes);
		void			handle_raw(int sender_fd, int nbytes);

		void			arr_to_list(void);
		void			list_to_arr(void);
		void			poll_loop(void);
		void			polling(void);
		
		void			check_activity(void);
		/* accessors */
		std::string		get_password(void);

	private:

		std::string			_message;
		std::string			_password;
		std::string			_port;
		std::string			_address;

		std::list<pollfd>	_pfds;
		struct pollfd		*_arr_pfds;

		std::map<int, User *> _User_list;

		char				_buf[510];
		int					_listener;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif