#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"

void    PASS(User *user)
{
    // std::cout << user->get_server()->get_password() << std::endl;
    if (user->get_status() == "registered")
        return (user->get_server()->to_send(
            ERR_ALREADYREGISTRED(getArgs(), user->get_nick()), user->get_fd()));
    if (user->param_list.size() == 0)
    {
        return (user->get_server()->to_send(
            ERR_NEEDMOREPARAMS(getArgs("PASS"), user->get_nick()), user->get_fd()));
    }
	if (user->get_server()->get_password() != user->param_list[0])
    {
        std::cout << RED "FAILED TO CONNECT : wrong password" E << std::endl;
		user->set_status("out");
    }
    else
    {
        std::cout << GREEN "CONNECTED/REGISTERED !" E << std::endl;
        // user->set_status("connected/registered");
    }
    // set_password(User->param_list[0]);
}
