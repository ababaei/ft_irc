#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include "Server.hpp"

class User
{
	public:
        User(int fd);
        ~User();

	private:
        std::string nick;
        std::string status;
        std::string message;

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif
