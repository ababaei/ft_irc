#include "command.hpp"
#include "colors.hpp"

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
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
    }

	Channel* chan = NULL;
	User* invited = NULL;
	std::string invitedName = "";
	std::string chanName = "";

    if (isChanName(user->param_list[0]))
	{
		chanName = user->param_list[0];
		invitedName = user->param_list[1];
		chan = user->getServer()->getChannel(user->param_list[0]);
		invited = user->getServer()->getUser(user->param_list[1]);
	}
	else if (isChanName(user->param_list[1]))
	{
		chanName = user->param_list[1];
		invitedName = user->param_list[0];
		chan = user->getServer()->getChannel(user->param_list[1]);
		invited = user->getServer()->getUser(user->param_list[0]);
	}
	else
		chanName = user->param_list[0];

	if (chan == NULL)
	{
		std::cout << RED "Channel does not exist" E << std::endl; // rajouter reply
		return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(chanName), user->getNick()),
				user->getFd());
	}
	if (invited == NULL)
	{
		std::cout << RED "ERR_NOSUCHNICK" E << std::endl; // rajouter reply
		return user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(invitedName), user->getNick()),
				user->getFd());
	}
	if (chan->isHere(user->getNick()) == false)
	{
		std::cout << RED "ERR_NOTONCHANNEL you're not on that channel" E << std::endl; // rajouter reply
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(chan->getName()), user->getNick()),
				user->getFd());
	}
	if (chan->isHere(invited->getNick()))
	{
		std::cout << RED "ERR_USERONCHANNEL is already on that channel" E << std::endl; // rajouter reply
		return user->getServer()->toSend(ERR_USERONCHANNEL(getArgs(invited->getNick(), chan->getName()),
					user->getNick()), user->getFd());
	}
	if (chan->isChanOp(user->getNick()) == false)
	{
		std::cout << RED "ERR_CHANOPRIVSNEEDED is invite only and you're not ops" E << std::endl; // rajouter reply.
		return user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(chan->getName()),
					user->getNick()), user->getFd());
	}
	if (invited->getStatus() == "away")
		return user->getServer()->toSend(RPL_AWAY(getArgs(user->getNick(), user->getAway()), user->getNick()), user->getFd());
	std::cout << GREEN "RPL_INVITING is invited" E << std::endl; // rajouter reply.

	std::vector<int> fds;
	fds.push_back(user->getFd());
	fds.push_back(invited->getFd());
	user->getServer()->toSend(RPL_INVITING(getArgs(chan->getName(), invited->getNick()), user->getNick()),
		fds);
}
