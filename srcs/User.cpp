#include "../inc/User.hpp"
#include "../inc/command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

User::User(int fd) : fd(fd)
{
//    cmd_list["PASS"] = PASS;
	cmd_list["NICK"] = NICK;
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

	while (std::getline(ss, tmp, ' '))
	{
		words.push_back(tmp);
	}
	this->command = words[1];
//	std::copy(words.begin() + 1, words.end(), param_list.begin());

	std::cout << this->command << "\n";

	//this->exec_cmd();
}

void	User::exec_cmd()
{
	this->cmd_list[this->command](this->message);
}