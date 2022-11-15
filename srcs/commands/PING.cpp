#include "../../inc/command.hpp"
#include "../../inc/User.hpp"
#include "../../inc/Server.hpp"

void    PING(User *user)
{
    std::vector<std::string> params = user->param_list;

    if (params.size() == 0)
        return(user->get_server()->toSend(ERR_NOORIGIN(getArgs(), user->get_nick()), user->get_fd()));
    
    if (params[0] != user->get_server()->getServerName())
        return(user->get_server()->toSend(ERR_NOORIGIN(getArgs(user->get_server()->getAddress()), user->get_nick()), user->get_fd()));

    return(user->get_server()->toSend("PONG " + user->get_server()->getServerName(), user->get_fd()));
    //std::cout << "Hello PING" << std::endl;
}