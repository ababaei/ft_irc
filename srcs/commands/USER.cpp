#include "../../inc/command.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/colors.hpp"

// Quels protections necessaires vu que USER est utilise que 1 fois a la connexion ?
void USER(User *user)
{
	if (user->getNick() == "")
	{
		user->setStatus("out");
		return ;
	}
    std::cout << "Hello USER" << std::endl;
//    std::string msg = ":127.0.0.1 001 " + user->getNick() + "!ababaei@127.0.0.1\r\n";
 //   send(user->getFd(), msg.c_str(), msg.length(), 0);
    std::string username(user->param_list[1]);
    user->setUsername(user->param_list[1]);
    std::cout << BBLUE "Your uSER is: " << user->getUsername() << E << std::endl;

    // ---------------------
    // A Faire seulement lors du 1er 'uSER' lors de l'etablisement de la connexion
    // set hostname --> dans une fct separee ?
    user->setHostname(user->param_list[2]);
    std::cout << BBLUE "Your Hostname is: " << user->getHostname() << E << std::endl;

    // set realname, utile ? --> dans une fct separee ?
    std::string realname;
    for (unsigned int i = 3; i < user->param_list.size(); i++)
        realname = realname + user->param_list[i] + ' ';
    user->setRealName(realname);
    std::cout << BBLUE "Your realname is: " << user->getRealName() << E << std::endl;
    // ---------------------
    return (user->getServer()->toSend(RPL_WELCOME(user->param_list, user->getNick()), user->getFd()));
}
