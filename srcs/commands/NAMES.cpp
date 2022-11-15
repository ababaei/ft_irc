/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:02:35 by amontaut          #+#    #+#             */
/*   Updated: 2022/11/15 15:31:55 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/command.hpp"

void NAMES(User *user)
{
	std::cout << "hello Names" << std::endl;
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
	
	std::map<std::string, Channel *> channelList = user->get_server()->getChannelList();

	// if (user->param_list.size() == 0)
	// {
	// 	// Passe pas dans la boucle mais affiche direct qqch dans irssi

	// 	// Envoyes dans une serie de RPL_NAMEREPLY (syntaxe "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> ))
	// 	// afficher tout les chan et leur ocupant
	// 	// + a la fin, les users qui sont dans aucuns chans sont dans un 'channel *'
	// 	// + les users qui sont sur channel invisible sont dans un 'channel *'
	// 	// RPL_ENDOFNAMES pour marquer la fin de la reponse
	// }
	// else
	// {
	// 	for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	// 	{
	// 		std::cout << it->first << "   ";
	// 		std::vector<std::string> listNick = it->second->getNickList();
	// 		for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
	// 			std::cout << ' ' << *it << std::endl;
	// 	}

	// 	// 1 reply RPL_NAMEREPLY avec
	// 	// list all names sauf anonymous dans le chan specifie sauf si chan secret alors ca fait rien
	// 	// syntaxe "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
	// 	// suivi de RPL_ENDOFNAMES
	// }
}
