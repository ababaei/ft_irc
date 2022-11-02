/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:39:02 by ali               #+#    #+#             */
/*   Updated: 2022/11/01 16:23:36 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(const std::string &aName) : name(aName)
{
	topic = "";
	flags["secret"] = false;
	flags["moderated"] = false;
	flags["inviteOnly"] = false;
	flags["anonymous"] = false;

	key = "";
	userLimit = 500;
}

Channel::~Channel()
{
	flags.clear();
	inviteList.clear();
	banList.clear();
	users.clear();
}

const std::string &Channel::getName() const
{
	return name;
}

const std::string &Channel::getTopic() const
{
	return topic;
}

const std::string &Channel::getKey() const
{
	return key;
}

int Channel::getUserLimit() const
{
	return userLimit;
}

const std::vector<User *> Channel::getUserList() const
{
	std::vector<User *> userList;
	for (std::map<std::string, User *>::const_iterator it = users.begin();
		 it != users.end(); it++)
		userList.push_back(it->second);
	return userList;
}

const std::vector<std::string> Channel::getNickList() const
{
	std::vector<std::string> nickList;
	for (std::map<std::string, User *>::const_iterator it = users.begin();
		 it != users.end(); it++)
		nickList.push_back(it->first);
	return nickList;
}

int Channel::getUserNum() const
{
	return users.size();
}

const std::vector<int> Channel::getFds() const
{
	std::vector<int> fds;

	for (std::map<std::string, User *>::const_iterator it = users.begin();
		 it != users.end(); it++)
		fds.push_back(it->second->get_fd());
	return fds;
}

const std::vector<int> Channel::getOtherFds(const std::string &nick) const
{
	std::vector<int> fds;

	for (std::map<std::string, User *>::const_iterator it = users.begin();
		 it != users.end(); it++)
	{
		if (nick != it->second->get_nick())
			fds.push_back(it->second->get_fd());
	}
	return fds;
}

bool Channel::isInvited(const std::string &nick) const
{
	if (std::find(inviteList.begin(), inviteList.end(), nick) != inviteList.end())
		return true;
	return false;
}

bool Channel::isBanned(const std::string &nick) const
{
	if (std::find(banList.begin(), banList.end(), nick) != banList.end())
		return true;
	return false;
}

bool Channel::isChanOp(const std::string &nick)
{
	if (userModes[nick]["operator"] == true)
		return true;
	return false;
}

bool Channel::isVoiced(const std::string &nick)
{
	if (userModes[nick]["voiced"] == true)
		return true;
	return false;
}

bool Channel::isHere(const std::string &nick) const
{
	if (users.find(nick) != users.end())
		return true;
	return false;
}

bool Channel::isSecret()
{
	if (flags["secret"] == true)
		return true;
	return false;
}

bool Channel::isModerated()
{
	if (flags["moderated"] == true)
		return true;
	return false;
}

bool Channel::isInviteOnly()
{
	if (flags["inviteOnly"] == true)
		return true;
	return false;
}

bool Channel::isAnonymous()
{
	if (flags["anonymous"] == true)
		return true;
	return false;
}

bool Channel::hasOneOp()
{
	int count = 0;

	for (std::map<std::string, std::map<std::string, bool> >::iterator it = userModes.begin();
		 it != userModes.end(); it++)
	{
		if (it->second["operator"] == true)
			count++;
	}
	if (count > 1)
		return false;
	return true;
}

bool	Channel::canSpeak(const std::string& nick)
{
	if (!isHere(nick) || isBanned(nick) || (!isVoiced(nick) && isModerated()))
		return false;
	return true;
}

void	Channel::banUser(const std::string& nick)
{
	banList.push_back(nick);
	users.erase(nick);
	userModes.erase(nick);
}

void Channel::inviteUser(const std::string &nick)
{
	inviteList.push_back(nick);
}

void Channel::unbanUser(const std::string &nick)
{
	std::vector<std::string>::iterator it = std::find(banList.begin(), banList.end(), nick);
	if (it != banList.end())
		banList.erase(it);
}

void Channel::uninviteUser(const std::string &nick)
{
	std::vector<std::string>::iterator it = std::find(inviteList.begin(), inviteList.end(), nick);
	if (it != inviteList.end())
		inviteList.erase(it);
}

void Channel::kickUser(const std::string &nick)
{
	users.erase(nick);
	userModes.erase(nick);
}

void Channel::addUser(User *user)
{
	std::string nick = user->get_nick();
	users[nick] = user;
	if (userModes.empty())
		userModes[nick]["operator"] = true;
	else
		userModes[nick]["operator"] = false;
	userModes[nick]["voiced"] = false;
}

void Channel::setName(const std::string &aName)
{
	name = aName;
}

void Channel::setTopic(const std::string &aTopic)
{
	topic = aTopic;
}

void Channel::setKey(const std::string &aKey)
{
	key = aKey;
}

void Channel::setUserLimit(int aUserLimit)
{
	userLimit = aUserLimit;
}

void Channel::setChanFlag(const std::string &flag, bool b)
{
	flags[flag] = b;
}

void Channel::setUserOp(const std::string &nick, bool b)
{
	userModes[nick]["operator"] = b;
}

void Channel::setUserVoiced(const std::string &nick, bool b)
{
	userModes[nick]["voiced"] = b;
}
