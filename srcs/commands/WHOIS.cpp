#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"

void send_infos(User *user, std::string nick, std::string username, std::string realname, std::string host,
std::vector<std::string> params)
{
    user->get_server()->to_send(nick + " " + username + " " + host + " * :" + realname, user->get_fd());
    
    user->get_server()->to_send(nick + " " + user)
}

void    WHOIS(User *user)
{
    std::vector<std::string> params = user->param_list;
    
    if (params.size() == 0)
        return (user->get_server()->to_send(ERR_NONICKNAMEGIVEN(getArgs(0),
        user->get_nick()), user->get_fd()));
    else
    {    
        User user_info;
        user_info->set_nick(params[0]);
    }

    if (params.size() > 1 && params[0] != user->get_server()->get_address())
        return (user->get_server()->to_send(ERR_NOSUCHSERVER(getArgs(1, params[0]),
        user->get_nick()), user->get_fd()));

    if (user->get_server()->isHere(params[0]) == false)
        return (user->get_server()->to_send(ERR_NOSUCHNICK(getArgs(1, params[0]),
        user->get_nick()), user->get_fd()));

    return(send_infos(user, params[0]->get_nick(), user->get_username(), user->get_real_name(), user->get_hostname(), 
        params));
}