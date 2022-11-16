#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"

void    PASS(User *user)
{
    if (user->getStatus() == "registered" || user->getStatus() == "pending")
        return (user->getServer()->toSend(
            ERR_ALREADYREGISTRED(getArgs(), user->getNick()), user->getFd()));
    if (user->param_list.size() == 0)
    {
        return (user->getServer()->toSend(
            ERR_NEEDMOREPARAMS(getArgs("PASS"), user->getNick()), user->getFd()));
    }
	if (user->getServer()->getPassword() != user->param_list[0])
    {
        std::cout << RED "FAILED TO CONNECT : wrong password" E << std::endl;
		user->setStatus("out");
    }
    else
    {
        std::cout << GREEN "CONNECTED/REGISTERED !" E << std::endl;
        user->setStatus("pending");
    }
    // set_password(User->param_list[0]);
}