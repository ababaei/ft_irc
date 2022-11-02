/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/02 12:57:29 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"

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
	//  461    ERR_NEEDMOREPARAMS
	//  ERR_CHANOPRIVSNEEDED = quand pas les droits if des droits are needed
	//  ERR_NOTONCHANNEL = quand on veut changer topic mais qu'on est pas sur le chan
	//  ERR_NOCHANMODES = Channel doesn't support modes" // ??
		
	//	/TOPIC = no topic set (affiche ca ou le topic)
	// /TOPIC lala = #toto :lala
	// /TOPIC #toto lala = #toto :lala

	for (std::vector<std::string>::iterator it = user->param_list.begin();  it != user->param_list.end(); it++)
		std::cout << ' ' << *it;
	std::cout << std::endl;
		
}
