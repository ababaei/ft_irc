/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 12:29:22 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "colors.hpp"

void TOPIC(User *user)
{
	// La commande /topic te permet de changer le topic du channel (titre d'un maximum de 80 caractères).
	// Si le channel est en mode +t, seule les opérateurs peuvent changer le topic.

	// 	 Parameters: <channel> [ <topic> ]

	// The topic for channel <channel> is returned if there is no <topic> given.
	// If the <topic> parameter is present, the topic for that channel will be changed, if this action is allowed for the user
	//    requesting it.
	// If the <topic> parameter is an empty string, the topic for that channel will be removed. // ???

	// ERR :
	//  461    ERR_NEEDMOREPARAMS //cest a dire ? Quand faut il + de param ?
	//  OK ERR_CHANOPRIVSNEEDED = quand pas les droits if des droits are needed
	//  OK ERR_NOTONCHANNEL = quand on est pas sur le chan
	//  ERR_NOCHANMODES = Channel doesn't support modes" // ??

	//	/TOPIC = no topic set (affiche ca ou le topic)
	// /TOPIC lala = #toto :lala
	// /TOPIC #toto lala = #toto :lala

	if (user->param_list.size() < 2)
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	std::string channel = user->param_list[0];
	std::string newtopic = user->param_list[1].erase(0, 1);
	if (newtopic.length() > 80)
	{
		std::cout << RED "Topic's name is too long" E << std::endl; // Pas dans rfc mais sur https://www.techbull.com/techbull/guide/internet/irccommande.html 
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	}

	Channel* chan = user->getServer()->getChannel(channel);
	if (chan == NULL)
	{
		std::cout << RED "Channel doesnt exist" E << std::endl; // utile ?
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	}

	if (chan->isHere(user->getNick()) == false)
	{
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(channel), user->getNick()),
				user->getFd());
	}

	// SI chan en mode +t , alors only ops can change subject.
	if ((chan->isTopicOperatorOnly() && chan->isChanOp(user->getNick()) == 1)
			|| chan->isTopicOperatorOnly() == false)
	{
		chan->setTopic(newtopic);
		user->getServer()->toSend(RPL_TOPIC(getArgs(channel, newtopic), user->getNick()), chan->getFds());
	}
	else
	{
		user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(channel), user->getNick()), user->getFd());
	}
}
