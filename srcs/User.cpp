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
	std::cout << this->param_list[0] << "\n";

	this->exec_cmd();
}

void	User::exec_cmd()
{
	if (this->command == "CAP")
		return ;
	std::cout << this->command << "\n";
	this->cmd_list[this->command](this->message);
}