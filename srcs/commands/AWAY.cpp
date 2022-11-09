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
        user->set_away(user->param_list[0].erase(0, 1));
        user->get_server()->to_send(RPL_NOWAWAY(getArgs(""),
                                                user->get_nick()),
                                    user->get_fd());
        // std::cout << "RPL_NOWAWAY" << std::endl;
    }
    else
    {
        user->set_mode("away", false);
         user->set_status("");
        user->set_away("");
        user->get_server()->to_send(RPL_UNAWAY(getArgs(""),
                                               user->get_nick()),
                                    user->get_fd());
        // std::cout << "RPL_UNAWAY" << std::endl;
    }
}
