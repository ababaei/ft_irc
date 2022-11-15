#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"

void    PASS(User *user)
{
    std::cout << user->getServer()->getPassword() << std::endl;
	if (user->getServer()->getPassword() != user->param_list[0])
    {
        std::cout << RED "FAILED TO CONNECT : wrong password" E << std::endl;
		user->setStatus("out");

    }
    else
    {
        std::cout << GREEN "CONNECTED/REGISTERED !" E << std::endl;
        user->setStatus("connected/registered");
    }
    // set_password(User->param_list[0]);
}
