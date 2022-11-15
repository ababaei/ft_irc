#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"

void AWAY(User *user)
{
    // syntax : AWAY text 
    // ou sans text = remove away

    // replies :
    // RPL_NOWAWAY = marked as away
    // RPL_UNAWAY    = plus away

    if (user->param_list[0].size() > 1)
    {
        user->set_mode("away", true);
         user->set_status("away");

		std::vector<std::string> msg(user->param_list.begin() + 1, user->param_list.end());
		user->set_away(getStr(msg));
        user->get_server()->toSend(RPL_NOWAWAY(getArgs(""),
                                                user->get_nick()),
                                    user->get_fd());
		user->get_server()->toSend(RPL_UMODEIS(getArgs("+a"), user->get_nick()),
					user->get_fd());
        // std::cout << "RPL_NOWAWAY" << std::endl;
    }
    else
    {
        user->set_mode("away", false);
         user->set_status("");
        user->set_away("");
        user->get_server()->toSend(RPL_UNAWAY(getArgs(""),
                                               user->get_nick()),
                                    user->get_fd());
		user->get_server()->toSend(RPL_UMODEIS(getArgs("-a"), user->get_nick()),
					user->get_fd());
        // std::cout << "RPL_UNAWAY" << std::endl;
    }
}
