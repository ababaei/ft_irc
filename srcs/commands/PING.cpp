#include "../../inc/command.hpp"
#include "../../inc/User.hpp"
#include "../../inc/Server.hpp"

void    PING(User *user)
{
    std::vector<std::string> params = user->param_list;

    if (params.size() == 0)
        return(user->getServer()->toSend(ERR_NOORIGIN(getArgs(), user->getNick()), user->getFd()));
    
    if (params[0] != user->getServer()->getServerName())
        return(user->getServer()->toSend(ERR_NOORIGIN(getArgs(user->getServer()->getAddress()), user->getNick()), user->getFd()));

    return(user->getServer()->toSend("PONG :" + user->getNick(), user->getFd()));
    //std::cout << "Hello PING" << std::endl;
}
