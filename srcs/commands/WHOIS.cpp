#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"

void send_infos(User *user, std::string nick, std::string username, std::string realname, std::string host,
time_t idle)
{
    user->get_server()->to_send(RPL_WHOISUSER(getArgs(nick, username, host, realname), nick), user->get_fd());
    if (user->get_status() == "inactive")
        user->get_server()->to_send(RPL_WHOISIDLE(getArgs(nick, convert(idle)), nick), user->get_fd());
    else if (user->get_status() == "away")
        user->get_server()->to_send(RPL_AWAY(getArgs(nick, "is away from keyboard."), nick), user->get_fd());
    user->get_server()->to_send(RPL_ENDOFWHOIS(getArgs(nick), nick), user->get_fd());
}

void    WHOIS(User *user)
{
    std::vector<std::string> params = user->param_list;
    
    std::cout << "size: " << params.size() << " " << params[0] << "\n";
    if (params.size() == 0)
        return (user->get_server()->to_send(ERR_NONICKNAMEGIVEN(getArgs(),user->get_nick()),
        user->get_fd()));
    else
    {
        User *user_info = user->get_server()->get_user(params[0]);

        if (user_info == NULL)
            return (user->get_server()->to_send(ERR_NOSUCHNICK(getArgs(params[0]),
            params[0]), user->get_fd()));

        if (params.size() > 1 && params[0] != user->get_server()->get_address())
            return (user->get_server()->to_send(ERR_NOSUCHSERVER(getArgs(params[0]),
            params[0]), user->get_fd()));

        return(send_infos(user, user_info->get_nick(), user_info->get_username(), user_info->get_real_name(), user_info->get_hostname(), 
            user_info->get_activity()));
    }
}
