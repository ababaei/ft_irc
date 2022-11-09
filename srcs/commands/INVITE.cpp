#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"
#include "../../inc/colors.hpp"

//    Parameters: <nickname> <channel>

// if the channel exists, only members of the channel
//    are allowed to invite other users.  When the channel has invite-only
//    flag set, only channel operators may issue INVITE command.

// Only the user inviting and the user being invited will receive
//    notification of the invitation.

// OK ERR_NEEDMOREPARAMS = manque param
// OK ERR_NOSUCHNICK = No such nick/channel
// OK ERR_NOTONCHANNEL = You're not on that channel
// OK ERR_USERONCHANNEL = <user> <channel> :is already on channel
// OK ERR_CHANOPRIVSNEEDED = <channel> :You're not channel operator
// OK RPL_INVITING = <channel> <nick>" Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client.
// OK RPL_AWAY = "<nick> :<away message>"

void INVITE(User *user)
{
    if (user->param_list.size() < 2)
    {
        std::cout << RED "ERR_NEEDMOREPARAMS" E << std::endl; // rajouter reply
		return user->get_server()->to_send(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->get_nick()),
				user->get_fd());
    }

    // chan en 1er
	Channel* chan;
    if (isChanName(user->param_list[1]))
		chan = user->get_server()->get_channel(user->param_list[0]);
	else
		return ;

	if (chan == NULL)
	{
		std::cout << RED "Channel does not exist" E << std::endl; // rajouter reply
		return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(chan->getName()), user->get_nick()),
				user->get_fd());
	}
	User* invited = user->get_server()->get_user(user->param_list[0]);
	if (invited == NULL)
	{
		std::cout << RED "ERR_NOSUCHNICK" E << std::endl; // rajouter reply
		return user->get_server()->to_send(ERR_NOSUCHNICK(getArgs(invited->get_nick()), user->get_nick()),
				user->get_fd());
	}
	if (chan->isHere(user->get_nick()) == false)
	{
		std::cout << RED "ERR_NOTONCHANNEL you're not on that channel" E << std::endl; // rajouter reply
		return user->get_server()->to_send(ERR_NOTONCHANNEL(getArgs(chan->getName()), user->get_nick()),
				user->get_fd());
	}
	if (chan->isHere(invited->get_nick()))
	{
		std::cout << RED "ERR_USERONCHANNEL is already on that channel" E << std::endl; // rajouter reply
		return user->get_server()->to_send(ERR_USERONCHANNEL(getArgs(invited->get_nick(), chan->getName()),
					user->get_nick()), user->get_fd());
	}
	if (chan->isChanOp(user->get_nick()) == false)
	{
		std::cout << RED "ERR_CHANOPRIVSNEEDED is invite only and you're not ops" E << std::endl; // rajouter reply.
		return user->get_server()->to_send(ERR_CHANOPRIVSNEEDED(getArgs(chan->getName()),
					user->get_nick()), user->get_fd());
	}
	if (invited->get_status() == "away")
		return user->get_server()->to_send(RPL_AWAY(getArgs(user->get_nick(), "is away from keyboard."), user->get_nick()), user->get_fd());
	std::cout << GREEN "RPL_INVITING is invited" E << std::endl; // rajouter reply.

	std::vector<int> fds;
	fds.push_back(user->get_fd());
	fds.push_back(invited->get_fd());
	user->get_server()->to_send(RPL_INVITING(getArgs(chan->getName(), invited->get_nick()), user->get_nick()),
		fds);
}
