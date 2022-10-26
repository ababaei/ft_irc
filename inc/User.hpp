#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include "Server.hpp"

class Server;

class User
{
	public:
        User(int fd, Server *server);
        ~User();

        int             get_fd();
        std::string     get_nick();
        std::string     get_username();
        std::string     get_real_name();
        std::string     get_hostname();
        std::string     get_status();
        Server          *get_server();

        void            set_nick(std::string nick);
        void            set_username(std::string username);
        void            set_hostname(std::string hostname);
        void            set_real_name(std::string real_name);
        void            set_status(std::string real_name);
        void            to_command(std::string msg);
        std::string     message;
        std::vector<std::string> param_list;

	private: //variables
        int             _fd;
        std::string     _nick;
        std::string     _username;
        std::string     _hostname;
        std::string     _real_name;
        std::string     _status; //connected ou enregistre / disconnected / en attente de connexion / inactif
        Server          *_server;

        std::string     _command;

        std::map<std::string, void (*)(User *)> _cmd_list;
        
        private: //method
        void			exec_cmd(void);          
        void                    clear_cmd(void);
/*
inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN)
*/
};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif
