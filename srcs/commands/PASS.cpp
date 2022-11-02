#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"

void    PASS(User *user)
{
    std::cout << user->get_server()->get_password() << std::endl;
	if (user->get_server()->get_password() != user->param_list[0])
    {
        std::cout << RED "FAILED TO CONNECT : wrong password" E << std::endl;
	    exit(0); //Quitter de maniere plus propre
    }
    else
    {
        std::cout << GREEN "CONNECTED/REGISTERED !" E << std::endl;
        user->set_status("connected/registered");
    }
    // set_password(User->param_list[0]);
}
