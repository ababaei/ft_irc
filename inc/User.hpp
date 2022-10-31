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

        int             get_fd(void);
        std::string     get_nick(void);
        std::string     get_username(void);
        std::string     get_real_name(void);
        std::string     get_hostname(void);
        std::string     get_status(void);
        time_t          get_activity(void);
        Server          *get_server(void);
		bool			get_mode(const std::string& mode);

        void            set_nick(std::string nick);
        void            set_username(std::string username);
        void            set_hostname(std::string hostname);
        void            set_real_name(std::string real_name);
        void            set_status(std::string status);
		void			set_mode(const std::string& mode, bool b);

        void            to_command(std::string msg);
        void            update_activity(void);

        std::string     message;
        std::vector<std::string> param_list;

	private: //variables
        int             _fd;
        std::string     _nick;
        std::string     _username;
        std::string     _hostname;
        std::string     _real_name;
        std::string     _status; //connected ou enregistre / disconnected / en attente de connexion / inactif
        std::map<std::string, bool> modes; //ex: modes["away"] = true indique que le user est away
        Server          *_server;
        std::vector<std::string> _chanels; 

        time_t _last_activity;

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
