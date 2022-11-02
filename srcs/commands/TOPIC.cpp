/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/02 16:05:03 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"

void TOPIC(User *user)
{
	std::cout << "hello TOPIC" << std::endl;
	// La commande /topic te permet de changer le topic du channel (titre d'un maximum de 80 caractères).
	// Le topic est là pour présenter le channel, ou montrer le sujet de discussion.
	// Si le channel est en mode +t, seule les opérateurs peuvent changer le topic.
	//  Si tu ne spécifies pas de message de topic, le serveur te retournera le topic actuel du channel spécifié.

	// 	 Parameters: <channel> [ <topic> ]

	//    The TOPIC command is used to change or view the topic of a channel.
	//    The topic for channel <channel> is returned if there is no <topic>
	//    given.  If the <topic> parameter is present, the topic for that
	//    channel will be changed, if this action is allowed for the user
	//    requesting it.  If the <topic> parameter is an empty string, the
	//    topic for that channel will be removed.

	// ERR :
	//  461    ERR_NEEDMOREPARAMS //cest a dire ? Quand faut il + de param ?
	//  ERR_CHANOPRIVSNEEDED = quand pas les droits if des droits are needed
	//  ERR_NOTONCHANNEL = quand on est pas sur le chan
	//  ERR_NOCHANMODES = Channel doesn't support modes" // ??

	//	/TOPIC = no topic set (affiche ca ou le topic)
	// /TOPIC lala = #toto :lala
	// /TOPIC #toto lala = #toto :lala

	std::string chanel = user->param_list[0].erase(0, 1);
	std::string newtopic = user->param_list[1].erase(0, 1);

	if (user->get_server()->get_channel_list().count(chanel) != 1) // utile ?
	{
		std::cout << "Passe channel doesnt exist" << std::endl;
		return;
	}
	// std::cout << RED "Chanel doesnt exist" E << std::endl; //utile ?

	std::vector<User *> userList = user->get_server()->get_channel(chanel)->getUserList();
	int i = 0;
	while (i < userList.size())
	{
		std::cout << "Passe channel check your in the can" << std::endl;

		while (user->get_username() != userList[i]->get_username())
			i++;
		if (user->get_username() == userList[i]->get_username())
			break;
		if (i == userList.size())
			std::cout << "ERR_NOTONCHANNEL = You're not in the chan" << std::endl;
	}

	// SI chan en mode +t , alors only ops can change subject.
	// I guess qu'on sait si le chan est en mode + t avec la fct isModerated ?? TBC with Arthur
	if (user->get_server()->get_channel(chanel)->isModerated() == 1 && user->get_server()->get_channel(chanel)->isChanOp(user->get_nick()) == 1) // on check si qqn est ops avec sur username ou nickname ?
	{
		std::cout << "Passe moderated and ok" << std::endl;
		user->get_server()->get_channel(chanel)->setTopic(newtopic);
		std::cout << YELLOW "New topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}
	else if (user->get_server()->get_channel(chanel)->isModerated() == 0)
	{
		std::cout << "Passe no moderation" << std::endl;
		user->get_server()->get_channel(chanel)->setTopic(newtopic);
		std::cout << YELLOW "New topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}
	else
		std::cout << "ERR_CHANOPRIVSNEEDED you dont have the rights" << std::endl;

	if (user->get_server()->get_channel(chanel)->getTopic() == "")
	{
		std::cout << "Passe no topic" << std::endl;
		std::cout << RED "No topic" E << std::endl;
	}
	else
	{
		std::cout << "Passe topic is" << std::endl;
		std::cout << YELLOW "Topic is:" << user->get_server()->get_channel(chanel)->getTopic() << E << std::endl;
	}

	for (std::vector<std::string>::iterator it = user->param_list.begin(); it != user->param_list.end(); it++)
		std::cout << ' ' << *it;
	std::cout << std::endl;
}
