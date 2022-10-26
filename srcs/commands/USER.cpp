#include "../../inc/command.hpp"

void    USER(User *User)
{
    std::cout << "Hello USER" << std::endl;
    std::string msg = ":127.0.0.1 001 " + User->get_nick() + "!ababaei@127.0.0.1\r\n"; 
    send(User->get_fd(), msg.c_str(), msg.length(), 0);
    User->set_username(User->param_list[0]);
    //ajouter protection si no args
    //send reply
    std::cout << "VAR USER IS : " << User->get_username() << std::endl;
}