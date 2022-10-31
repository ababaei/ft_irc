#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void    WHOIS(User *user)
{
    std::vector<std::string> params = user->param_list;
    if (params.size() == 0)
        return (user->get_server()->to_send(ERR_NONICKNAMEGIVEN(getArgs(0), user->get_nick())));
    std::cout << "Hello WHOIS" << std::endl;
}