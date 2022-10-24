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
    //

    User->set_nick(nick);
}