#include "../../inc/command.hpp"

void    USER(User *User)
{
    std::cout << "Hello USER" << std::endl;
    std::string msg = ":127.0.0.1 001 " + User->get_nick() + "!ababaei@127.0.0.1\r\n"; 
    send(User->get_fd(), msg.c_str(), msg.length(), 0);
    std::string username(User->param_list[1]);
    User->set_username(User->param_list[1]);
    //ajouter protection si no args
    std::cout << "VAR USER IS : " << User->get_username() << std::endl;

    // ---------------------
//A Faire seulement lors du 1er 'USER' lors de l'etablisement de la connexion
	//set hostname --> dans une fct separee ? 
	User->set_hostname(User->param_list[2]);
    std::cout << "VAR HOSTNAME IS : " << User->get_hostname() << std::endl;

	//set hostname, utile ? --> dans une fct separee ? 
	std::string realname;
	for (int i = 3; i < User->param_list.size(); i++)
		realname = realname + User->param_list[i] + ' ';

	User->set_real_name(realname);
    std::cout << "VAR REALNAME IS : " << User->get_real_name() << std::endl;
// ---------------------
}