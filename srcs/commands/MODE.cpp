#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"


void	chanMode(User* user, std::vector<std::string& params)
{
	

}

void	userMode(User* user, std::vector<std::string>& params)
{

}

void    MODE(User *user)
{
	std::vector<std::string> params = user->param_list;
	if (params.size() < 2)
		user->get_server()->to_send(ERR_NEEDMOREPARAMS("MODE", user->get_nick()),
				user->get_fd());
	if (isChanName(params[0]))
		chanMode(user, params);
	else
		userMode(user, params);
}
