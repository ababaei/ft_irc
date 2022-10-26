#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void    PASS(User *User)
{
    std::cout << User->get_server()->get_password() << std::endl;
	if (User->get_server()->get_password() != User->param_list[0])
        std::cout << "FAILED TO CONNECT : wrong password" << std::endl;
    else
        std::cout << "CONNECTED/REGISTERED !" << std::endl;
    // set_password(User->param_list[0]);
}