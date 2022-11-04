/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/04 11:18:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"

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

	std::string chanel = user->param_list[0];
	std::string newtopic = user->param_list[1].erase(0, 1);
	if (newtopic.length() > 80)
	{
		std::cout << RED "Topic's name is too long" E << std::endl; // Pas dans rfc mais sur https://www.techbull.com/techbull/guide/internet/irccommande.html 
		return;
	}

	if (user->get_server()->get_channel_list().count(chanel) != 1) // utile ?
	{
		std::cout << RED "Chanel doesnt exist" E << std::endl; // utile ?
		return;
	}

	std::vector<User *> userList = user->get_server()->get_channel(chanel)->getUserList();
	unsigned int i = 0;
	while (i < userList.size())
	{
		while (user->get_username() != userList[i]->get_username())
			i++;
		if (user->get_username() == userList[i]->get_username())
			break;
		if (i == userList.size())
			std::cout << "ERR_NOTONCHANNEL = You're not in the chan" << std::endl;
	}

	// SI chan en mode +t , alors only ops can change subject.
	if (user->get_server()->get_channel(chanel)->isTopicOperatorOnly() == 1 && user->get_server()->get_channel(chanel)->isChanOp(user->get_nick()) == 1) // on check si qqn est ops avec sur username ou nickname ?
	{
		user->get_server()->get_channel(chanel)->setTopic(newtopic);
		std::cout << YELLOW "New topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}
	else if (user->get_server()->get_channel(chanel)->isTopicOperatorOnly() == 0)
	{
		user->get_server()->get_channel(chanel)->setTopic(newtopic);
		std::cout << YELLOW "New topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}
	else
		std::cout << "ERR_CHANOPRIVSNEEDED you dont have the rights" << std::endl;

	if (user->get_server()->get_channel(chanel)->getTopic() == "")
	{
		std::cout << RED "No topic" E << std::endl;
	}
	else if (user->param_list.size() < 2)
	{
		std::cout << YELLOW "Topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}

	// for (std::vector<std::string>::iterator it = user->param_list.begin(); it != user->param_list.end(); it++)
	// 	std::cout << ' ' << *it;
	// std::cout << std::endl;
}
