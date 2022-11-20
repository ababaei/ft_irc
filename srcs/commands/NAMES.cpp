/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:02:35 by amontaut          #+#    #+#             */
/*   Updated: 2022/11/20 21:00:01 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"

// syntax : [ <channel> *( "," <channel> ) ]
// list all nicknames that are visible. CAD pas ceux qui sont en mode anonymous + When a channel is "secret", the server will act as if the channel does not exist
// The <channel> parameter specifies which channel(s) to return information about. There is no error reply for bad channel names.

// If no <channel> parameter is given, a list of all channels and their
// occupants is returned.  At the end of this list, a list of users who
// are visible but either not on any channel or not on a visible channel are listed as being on `channel' "*".

// target = pour plusieurs server
// replies :
//   RPL_NAMEREPLY =      "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
//  "@" is used for secret channels, "*" for private
//    channels, and "=" for others (public channels).
// rajouter @ devant chan operator names
// + devant nick = quand away ??

//  RPL_ENDOFNAMES =     "<channel> :End of NAMES list"
//  - To reply to a NAMES message, a reply pair consisting
//    of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
//    server back to the client.  If there is no channel
//    found as in the query, then only RPL_ENDOFNAMES is
//    returned.  The exception to this is when a NAMES
//    message is sent with no parameters and all visible
//    channels and contents are sent back in a series of
//    RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
//    the end.

void nameReply_names(User *user, Channel *chan)
{
	std::string reply_channel = " ";
	std::string reply_nick = " ";

	if (chan->isPrivate() == true && chan->isHere(user->getNick()) == true)
		reply_channel = "* " + chan->getName();
	else if (chan->isSecret() == true && chan->isHere(user->getNick()) == true)
		reply_channel = "@ " + chan->getName();
	else if (chan->isAnonymous() == true && chan->isHere(user->getNick()) == true)
		reply_channel = chan->getName();
	else if (chan->isAnonymous() == false && chan->isSecret() == false && chan->isPrivate() == false)
		reply_channel = "= " + chan->getName();
	else
		return;

	std::vector<std::string> nick_list = chan->getNickList();
	for (std::vector<std::string>::iterator it = nick_list.begin(); it != nick_list.end(); it++)
	{
		if (chan->isChanOp(*it))
			reply_nick += "@" + *it;
		else if (chan->isModerated() && chan->canSpeak(*it))
			reply_nick += "+" + *it;
		else
			reply_nick += " " + *it;
	}
	user->getServer()->toSend(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
											user->getNick()),
							  user->getFd());
	user->getServer()->toSend(RPL_ENDOFNAMES(getArgs(reply_channel), user->getNick()), user->getFd());
}

void NAMES(User *user)
{
	std::map<std::string, Channel *> channelList = user->getServer()->getChannelList();

	if (user->param_list.size() == 0)
	{
		std::cout << YELLOW "all chan + nick in the chan" E << std::endl;
		std::map<std::string, Channel *> channelsList = user->getServer()->getChannelList();
		for (std::map<std::string, Channel *>::iterator it = channelsList.begin(); it != channelsList.end(); ++it)
			nameReply_names(user, it->second);
		// + a la fin, les users qui sont dans aucuns chans sont dans un 'channel *'
		// + les users qui sont sur channel invisible sont dans un 'channel *'
	}
	else
	{
		Channel *chan = user->getServer()->getChannel(user->param_list[1]);
		if (chan == NULL)
			return (user->getServer()->toSend(RPL_ENDOFNAMES(getArgs(user->param_list[1]), user->getNick()), user->getFd()));
		if ((chan->isHere(user->getNick()) == true && chan->isPrivate() == true) || (chan->isHere(user->getNick()) == true && chan->isSecret() == true))
				nameReply_names(user, chan);
		if (chan->isSecret() == false && chan->isPrivate() == false && chan->isAnonymous() == false)
			nameReply_names(user, chan);
	}
}
