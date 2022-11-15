#include "command.hpp"

void send_infos(User *user, std::string nick, std::string username, std::string realname, std::string host,
time_t idle)
{
    user->getServer()->toSend(RPL_WHOISUSER(getArgs(nick, username, host, realname), nick), user->getFd());
    if (user->getStatus() == "inactive")
        user->getServer()->toSend(RPL_WHOISIDLE(getArgs(nick, convert(idle)), nick), user->getFd());
    else if (user->getStatus() == "away")
        user->getServer()->toSend(RPL_AWAY(getArgs(nick, user->getAway()), nick), user->getFd());
    user->getServer()->toSend(RPL_ENDOFWHOIS(getArgs(nick), nick), user->getFd());
}

void    WHOIS(User *user)
{
    std::vector<std::string> params = user->param_list;
    
    std::cout << "size: " << params.size() << " " << params[0] << "\n";
    if (params.size() == 0)
        return (user->getServer()->toSend(ERR_NONICKNAMEGIVEN(getArgs(),user->getNick()),
        user->getFd()));
    else
    {
        User *user_info = user->getServer()->getUser(params[0]);

        if (user_info == NULL)
            return (user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(params[0]),
            params[0]), user->getFd()));

        if (params.size() > 1 && params[0] != user->getServer()->getAddress())
            return (user->getServer()->toSend(ERR_NOSUCHSERVER(getArgs(params[0]),
            params[0]), user->getFd()));

        return(send_infos(user, user_info->getNick(), user_info->getUsername(), user_info->getRealName(), user_info->getHostname(), 
            user_info->getActivity()));
    }
}
