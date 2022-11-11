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
        return (user->get_server()->to_send(ERR_NEEDMOREPARAMS(
            getArgs("USER"), user->get_nick()), user->get_fd()
        ));
    if (user->get_status() == "registered")
    {
        return (user->get_server()->to_send(ERR_ALREADYREGISTRED(
            getArgs(), user->get_nick()), user->get_fd()
        ));
    }
    else
    {
        user->set_username(user->param_list[1]);
        std::cout << BBLUE "Your User is: " << user->get_username() << E << std::endl;

        // ---------------------
        // A Faire seulement lors du 1er 'uSER' lors de l'etablisement de la connexion
        // set hostname --> dans une fct separee ?
        user->set_hostname(user->param_list[2]);
        std::cout << BBLUE "Your Hostname is: " << user->get_hostname() << E << std::endl;

        // set realname, utile ? --> dans une fct separee ?
        std::string realname;
        for (unsigned int i = 3; i < user->param_list.size(); i++)
            realname.append(user->param_list[i] + ' ');
        realname.erase(realname.begin(), realname.end() - 1);
        if (checkRealName(realname))
            user->set_real_name(realname);
        std::cout << BBLUE "Your realname is: " << user->get_real_name() << E << std::endl;
        // ---------------------
        if (user->get_nick().size() != 0)
        {
            user->set_status("registered");
            return (user->get_server()->to_send(RPL_WELCOME(
                getArgs(user->get_nick(), user->get_username(), user->get_hostname()), user->get_nick()), user->get_fd()));
        }
    }
}