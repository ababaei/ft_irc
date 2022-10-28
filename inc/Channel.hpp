/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:32:51 by ali               #+#    #+#             */
/*   Updated: 2022/10/28 17:12:44 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <vector>
# include <iostream>

# include "User.hpp"

class User;

class Channel
{
	private:
		std::string	name;
		std::string	topic;
		std::map<std::string, bool>	flags;
		std::vector<std::string> inviteList;
		std::vector<std::string> banList;
		std::map< std::map<std::string, User*> users;
		std::map<std::string, std::map<std::string, bool> > userModes;
		// map to check for each user if voiced or if chanOp
		std::string	key;
		int	userLimit;

	public:
		Channel(const std::string& aName);
		~Channel();

		const std::string&	getName() const;
		const std::string&	getTopic() const;
		int	getKey() const;
		int	getUserLimit() const;
		const std::vector<User*>	getUserList() const;
		const std::vector<std::string> getNickList() const;
		int	getUserNum() const;

		bool	isInvited(const std::string& nick) const;
		bool	isBanned(const std::string& nick) const;
		bool	isChanOp(const std::string&	nick) const;
		bool	isVoiced(const std::string& nick) const;
		bool	isHere(const std::string& nick) const;

		bool	isSecret() const;
		bool	isModerated() const;
		bool	isInviteOnly() const;
		bool	isAnonymous() const;

		banUser(const std::string& nick);
		inviteUser(const std::string& nick);

		kickUser(const std::string &nick);
		addUser(User* user);

		setName(const std::string& aName);
		setTopic(const std::string& aTopic);
		setKey(int aKey);
		setUserLimit(const int& aUserLimit);
		setChanFlag(const std::string& flag, bool b);
		setUserOp(const std::string& nick, bool b);
		setUserVoiced(const std::string& nick, bool b);
};

#endif
