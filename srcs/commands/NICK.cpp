#include "../../inc/command.hpp"

void    NICK(User *User)
{
    std::string nick(User->param_list[0]);
    User->set_nick(nick);
    //ajouter protection si no args
    //send reply
    std::cout << "VAR NICK IS : " << User->get_nick() << std::endl;
}