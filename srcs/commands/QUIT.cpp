#include "command.hpp"

void	QUIT(User* user)
{
	Server*	serv = user->get_server();
	std::vector<std::string> chanList = user->getChannelList();

	for (std::vector<std::string>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		serv->get_channel(*it)->kickUser(user->get_nick());
		if (serv->get_channel(*it)->getUserNum() == 0)
			serv->deleteChannel(*it);
	}
	user->set_status("out");
}
