#include "command.hpp"
#include "Server.hpp"
#include "replies.hpp"
#include "utils.hpp"

void AWAY(User *user)
{
    // syntax : AWAY text 
    // ou sans text = remove away

    // replies :
    // RPL_NOWAWAY = marked as away
    // RPL_UNAWAY    = plus away

    if (user->param_list[0].size() > 0  && user->param_list.size() > 0)
    {
        user->setMode("away", true);
         user->setStatus("away");

		// std::vector<std::string> msg(user->param_list.begin(), user->param_list.end());
        if (user->param_list[0][0] == ':')
            user->param_list[0] = user->param_list[0].substr(1, user->param_list[0].size());
        user->setAway(getStr(user->param_list));
        user->getServer()->toSend(RPL_NOWAWAY(getArgs(""),
                                                user->getNick()),
                                    user->getFd());
		user->getServer()->toSend(RPL_UMODEIS(getArgs("+a"), user->getNick()),
					user->getFd());
        // std::cout << "RPL_NOWAWAY" << std::endl;
    }
    else
    {
        user->setMode("away", false);
        user->setStatus("");
        user->setAway("");
        user->getServer()->toSend(RPL_UNAWAY(getArgs(""),
                                               user->getNick()),
                                    user->getFd());
		user->getServer()->toSend(RPL_UMODEIS(getArgs("-a"), user->getNick()),
					user->getFd());
        // std::cout << "RPL_UNAWAY" << std::endl;
    }
}
