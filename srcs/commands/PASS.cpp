#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void    PASS(User *User)
{
    std::cout << User->get_server()->get_password() << "\n";
    // set_password(User->param_list[0]);
}