#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"

// Quels protections necessaires vu que USER est utilise que 1 fois a la connexion ?
void USER(User *User)
{
    std::cout << "Hello USER" << std::endl;
    std::string msg = ":127.0.0.1 001 " + User->get_nick() + "!ababaei@127.0.0.1\r\n";
    send(User->get_fd(), msg.c_str(), msg.length(), 0);
    std::string username(User->param_list[1]);
    User->set_username(User->param_list[1]);
    std::cout << BBLUE "Your USER is: " << User->get_username() << E << std::endl;

    // ---------------------
    // A Faire seulement lors du 1er 'USER' lors de l'etablisement de la connexion
    // set hostname --> dans une fct separee ?
    User->set_hostname(User->param_list[2]);
    std::cout << BBLUE "Your Hostname is: " << User->get_hostname() << E << std::endl;

    // set realname, utile ? --> dans une fct separee ?
    std::string realname;
    for (int i = 3; i < User->param_list.size(); i++)
        realname = realname + User->param_list[i] + ' ';
    User->set_real_name(realname);
    std::cout << BBLUE "Your realname is: " << User->get_real_name() << E << std::endl;
    // ---------------------
}