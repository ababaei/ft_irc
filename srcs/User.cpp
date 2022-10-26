#include "../inc/User.hpp"
#include "../inc/command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

/*	The cmd_list's functions are declared in command.hpp and defined in their respective files.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
User::User(int fd, Server *server) : fd(fd), server(server)
{
	this->cmd_list["PASS"] = PASS;
	this->cmd_list["NICK"] = NICK;
	this->cmd_list["USER"] = USER;
	this->cmd_list["MODE"] = MODE;
	this->cmd_list["WHOIS"] = WHOIS;
	this->cmd_list["PING"] = PING;
}

User::~User() {}

int User::get_fd() { return (this->fd); }
std::string User::get_nick() { return (this->nick); }
std::string User::get_status() { return (this->status); }
Server *User::get_server() { return (this->server); }

void User::set_nick(std::string nick)
{
	this->nick = nick;
}

void User::to_command(std::string msg)
{
	std::stringstream ss(msg);
	std::vector<std::string> words;
	std::string tmp;

	this->message = msg;
	while (std::getline(ss, tmp, ' '))
	{
		words.push_back(tmp);
	}
	this->command = words[0];
	this->param_list.assign(words.begin() + 1, words.end());
	std::cout << "size=" << this->param_list.size() << "\n";
	std::cout << "words is:" << std::endl;
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
		std::cout << ' ' << *it;

	this->exec_cmd();
}

void User::clear_cmd(void)
{
	this->message.clear();
	this->command.clear();
	this->param_list.clear();
}

/* TO PROTECT
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void User::exec_cmd(void)
{
	if (this->command == "CAP")
		return;
	std::cout << "exe>" << this->command << "\n";
	this->cmd_list[this->command](this);
	this->clear_cmd();
}