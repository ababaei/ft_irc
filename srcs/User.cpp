#include "../inc/User.hpp"
#include "../inc/command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

/*	The cmd_list's functions are declared in command.hpp and defined in their respective files.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
User::User(int fd, Server *server) : _fd(fd), _server(server)
{
	this->_cmd_list["NICK"] = NICK;
	this->_cmd_list["PASS"] = PASS;
	this->_cmd_list["USER"] = USER;
	this->_cmd_list["MODE"] = MODE;
	this->_cmd_list["WHOIS"] = WHOIS;
	this->_cmd_list["PING"] = PING;
}

User::~User() {}

int User::get_fd() { return (this->_fd); }
std::string User::get_nick() { return (this->_nick); }
std::string User::get_username() { return (this->_username); }
std::string User::get_hostname() { return (this->_hostname); }
std::string User::get_real_name() { return (this->_real_name); }
std::string User::get_status() { return (this->_status); }
Server *User::get_server() { return (this->_server); }

void User::set_nick(std::string nick)
{
	this->_nick = nick;
}

void User::set_username(std::string username)
{
	this->_username = username;
}

void User::set_hostname(std::string hostname)
{
	this->_hostname = hostname;
}

void User::set_real_name(std::string real_name)
{
	this->_real_name = real_name;
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
	this->_command = words[0];
	this->param_list.assign(words.begin() + 1, words.end());
	// std::cout << "size=" << this->param_list.size() << "\n";
	std::cout << "size=" << this->param_list.size() << "\n";
	std::cout << "words is:" << std::endl;
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << " //" << std::endl;

	this->exec_cmd();
}

void User::clear_cmd(void)
{
	this->message.clear();
	this->_command.clear();
	this->param_list.clear();
}

/* TO PROTECT
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void User::exec_cmd(void)
{
	if (this->_cmd_list.find(this->_command) == this->_cmd_list.end())
	{
		std::cout << "> error: Command doesn't exist\n";
		return ;
	}
	else
	{
		std::cout << "exe>" << this->_command << "\n";
		this->_cmd_list[this->_command](this);
	}
	this->clear_cmd();
}