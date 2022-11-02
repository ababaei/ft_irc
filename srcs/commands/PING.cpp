#include "../../inc/command.hpp"
#include "../../inc/User.hpp"
#include "../../inc/Server.hpp"

void    PING(User *user)
{
    std::vector<std::string> params = user->param_list;

    if (params.size() == 0)
        return(user->get_server()->to_send(ERR_NOORIGIN(getArgs(0), user->get_nick()), user->get_fd()));
    
    if (params[0] != user->get_server()->get_server_name())
        return(user->get_server()->to_send(ERR_NOORIGIN(getArgs(1, user->get_server()->get_address()), user->get_nick()), user->get_fd()));

    return(user->get_server()->to_send("PONG " + user->get_server()->get_server_name(), user->get_fd()));
    //std::cout << "Hello PING" << std::endl;
}
