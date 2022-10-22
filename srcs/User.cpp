#include "../inc/User.hpp"
#include "../inc/command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

User::User(int fd) : fd(fd)
{
//    cmd_list["PASS"] = PASS;
	this->cmd_list["NICK"] = NICK;
}

User::~User() {}

int User::get_fd() { return (this->fd); }
std::string User::get_nick() { return (this->nick); }
std::string User::get_status() { return (this->status); }

void	User::set_nick(std::string nick)
{
	this->nick = nick;
}

void    User::to_command(std::string msg)
{
    std::stringstream			ss(msg);
	std::vector<std::string>	words;
	std::string					tmp;

	this->message = msg;
	while (std::getline(ss, tmp, ' '))
	{
		words.push_back(tmp);
	}
	this->command = words[0];
	this->param_list.assign(words.begin() + 1, words.end());
	//std::cout << this->param_list[0] << "\n";

	this->exec_cmd();
}

void	User::clear_cmd(void)
{
	this->message.clear();
	this->command.clear();
	this->cmd_list.clear();
}

void	User::exec_cmd(void)
{
	if (this->command == "CAP")
		return ;
	std::cout << "exe>" << this->command << "\n";
	this->cmd_list[this->command](this);
	this->clear_cmd();
}