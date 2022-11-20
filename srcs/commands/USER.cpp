#include "../../inc/command.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/colors.hpp"

bool	checkRealName(const std::string& param)
{
	if (param.size() < 1)
		return (false);
	return (true);
}

// Quels protections necessaires vu que USER est utilise que 1 fois a la connexion ?
void USER(User *user)
{
    // std::cout << "Hello USER" << std::endl;
    // std::string msg = ":127.0.0.1 001 " + user->get_nick() + "!ababaei@127.0.0.1\r\n";
    // send(user->get_fd(), msg.c_str(), msg.length(), 0);
    // std::string username(user->param_list[1]);
    if (user->param_list.size() < 4)
        return (user->getServer()->toSend(ERR_NEEDMOREPARAMS(
            getArgs("USER"), user->getNick()), user->getFd()
        ));
    if (user->getStatus() == "registered")
    {
        return (user->getServer()->toSend(ERR_ALREADYREGISTRED(
            getArgs(), user->getNick()), user->getFd()
        ));
    }
    else
    {
        user->setUsername(user->param_list[1]);
        std::cout << BBLUE "Your User is: " << user->getUsername() << E << std::endl;

        // ---------------------
        // A Faire seulement lors du 1er 'uSER' lors de l'etablisement de la connexion
        // set hostname --> dans une fct separee ?
        user->setHostname(user->param_list[2]);
        std::cout << BBLUE "Your Hostname is: " << user->getHostname() << E << std::endl;

        // set realname, utile ? --> dans une fct separee ?
        std::string realname;
        for (unsigned int i = 3; i < user->param_list.size(); i++)
            realname.append(user->param_list[i] + ' ');
        realname = realname.substr(1,realname.size() - 1);
        if (checkRealName(realname) == true)
            user->setRealName(realname);
        std::cout << BBLUE "Your realname is: " << user->getRealName() << E << std::endl;
        // ---------------------
        if (user->getNick().size() != 0)
        {
            user->setStatus("registered");
            return (user->getServer()->toSend(RPL_WELCOME(
                getArgs(user->getNick(), user->getUsername(), user->getHostname()), user->getNick()), user->getFd()));
        }
        else
        {
            user->setStatus("out");
            return ;
        }
    }
}