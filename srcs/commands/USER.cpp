#include "../../inc/command.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/colors.hpp"

// Quels protections necessaires vu que USER est utilise que 1 fois a la connexion ?
void USER(User *user)
{
    std::cout << "Hello USER" << std::endl;
    std::string msg = ":127.0.0.1 001 " + user->get_nick() + "!ababaei@127.0.0.1\r\n";
    send(user->get_fd(), msg.c_str(), msg.length(), 0);
    std::string username(user->param_list[1]);
    user->set_username(user->param_list[1]);
    std::cout << BBLUE "Your uSER is: " << user->get_username() << E << std::endl;

    // ---------------------
    // A Faire seulement lors du 1er 'uSER' lors de l'etablisement de la connexion
    // set hostname --> dans une fct separee ?
    user->set_hostname(user->param_list[2]);
    std::cout << BBLUE "Your Hostname is: " << user->get_hostname() << E << std::endl;

    // set realname, utile ? --> dans une fct separee ?
    std::string realname;
    for (unsigned int i = 3; i < user->param_list.size(); i++)
        realname = realname + user->param_list[i] + ' ';
    user->set_real_name(realname);
    std::cout << BBLUE "Your realname is: " << user->get_real_name() << E << std::endl;
    // ---------------------
    return (user->get_server()->to_send(RPL_WELCOME(user->param_list, user->get_nick()), user->get_fd()));
}
