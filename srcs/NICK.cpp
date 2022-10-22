#include "../inc/command.hpp"

void    NICK(User *User)
{
    std::string nick(User->param_list[0]);
    /*try
    {
        if (User->param_list.size() == 0)
            throw ERR_NONICKNAMEGIVEN();
        User->check_nick();
    }
    catch
    {
        
    }*/
    std::cout << "FD = " << User->get_fd() << "\n";

    User->set_nick(nick);
    std::cout << "NICK = " << User->get_nick() << "\n";
}