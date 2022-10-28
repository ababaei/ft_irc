#include "../../inc/command.hpp"
#include "../../inc/replies.hpp"

// Quels protections necessaires vu que USER est utilise que 1 fois a la connexion ?
void USER(User *user)
{
    std::cout << "Hello USER" << std::endl;
    std::string msg = ":127.0.0.1 001 " + user->get_nick() + "!ababaei@127.0.0.1\r\n";
    send(user->get_fd(), msg.c_str(), msg.length(), 0);
    std::string username(user->param_list[1]);
    user->set_username(user->param_list[1]);
    std::cout << "VAR USER IS : " << user->get_username() << std::endl;

    // ---------------------
    // A Faire seulement lors du 1er 'USER' lors de l'etablisement de la connexion
    // set hostname --> dans une fct separee ?
    user->set_hostname(user->param_list[2]);
    std::cout << "VAR HOSTNAME IS : " << user->get_hostname() << std::endl;

    // set realname, utile ? --> dans une fct separee ?
    std::string realname;
    for (int i = 3; i < user->param_list.size(); i++)
        realname = realname + user->param_list[i] + ' ';
    user->set_real_name(realname);
    std::cout << "VAR REALNAME IS : " << user->get_real_name() << std::endl;
    // ---------------------
    return (user->get_server()->to_send(RPL_WELCOME(user->param_list, user->get_nick()), user->get_fd()));
}