#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include "Server.hpp"

class User
{
	public:
        User(int fd);
        ~User();

        int             get_fd();
        std::string     get_nick();
        std::string     get_status();

        void            set_nick(std::string nick);
        void            to_command(std::string msg);
        std::vector<std::string> param_list;

	private: //variables
        int             fd;
        std::string     nick;
        std::string     status;
        std::string     message;

        std::string     command;
        std::string     sub_cmd;

        std::map<std::string, void (*)(User *)> cmd_list;
        
        private: //method
        void			exec_cmd(void);          
        void                    clear_cmd(void);
/*
inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN)
*/
};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif
