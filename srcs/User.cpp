#include "../inc/User.hpp"
#include "../inc/command.hpp"
#include "../inc/colors.hpp"

/*	The cmd_list's functions are declared in command.hpp and defined in their respective files.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
User::User(int fd, Server *server) : _fd(fd), _server(server)
{
	update_activity();
	this->_cmd_list["NICK"] = NICK;
	this->_cmd_list["PASS"] = PASS;
	this->_cmd_list["USER"] = USER;
	this->_cmd_list["MODE"] = MODE;
	this->_cmd_list["JOIN"] = JOIN;
	this->_cmd_list["WHOIS"] = WHOIS;
	this->_cmd_list["PING"] = PING;
	this->_cmd_list["PRIVMSG"] = PRIVMSG;
	this->_cmd_list["KICK"] = KICK;
	this->_cmd_list["PART"] = PART;
	this->_cmd_list["TOPIC"] = TOPIC;
	this->_cmd_list["LIST"] = LIST;
	this->_cmd_list["NAMES"] = NAMES;
	this->_cmd_list["INVITE"] = INVITE;
	this->_cmd_list["QUIT"] = QUIT;

	modes["away"] = false;
	modes["invisible"] = false;
	modes["wallops"] = false;
	modes["restricted"] = false;
	modes["operator"] = false;
	modes["local_op"] = false;
	modes["server_notice"] = false;
	_chanelsLimit = 10;
}

User::~User() {}

int User::get_fd() { return (this->_fd); }
std::string User::get_nick() { return (this->_nick); }
std::string User::get_username() { return (this->_username); }
std::string User::get_hostname() { return (this->_hostname); }
std::string User::get_real_name() { return (this->_real_name); }
std::string User::get_status() { return (this->_status); }
time_t User::get_activity(void) { return (this->_last_activity); }
Server *User::get_server() { return (this->_server); }
int User::getChanelLimit() const { return this->_chanelsLimit; }
const std::vector<std::string> User::getChannelList() const
{
	std::vector<std::string> channelList;
	for (std::vector<std::string>::const_iterator it = _chanels.begin();
		 it != _chanels.end(); it++)
		channelList.push_back(*it);
	return channelList;
}

bool	User::get_mode(const std::string& mode)
{
	return modes[mode];
}

void	User::set_nick(std::string nick)
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

void User::set_status(std::string status)
{
	this->_status = status;
}

void User::set_mode(const std::string &mode, bool b)
{
	modes[mode] = b;
}

void	User::setChanelLimit(int aChanelLimit)
{
	_chanelsLimit = aChanelLimit;
}

void User::add_channel(std::string new_channel)
{
	this->_chanels.push_back(new_channel);
}

void User::to_command(std::string msg)
{
	std::cout << CYAN "msg is " << msg << E << std::endl;
	std::stringstream ss(msg);
	std::vector<std::string> words;
	std::string tmp;

	this->message = msg;
	while (std::getline(ss, tmp, ' '))
	{
		words.push_back(tmp);
	}
	this->_command = words[0]; //SEGFAULT here when msg is "\r\n"
	this->param_list.assign(words.begin() + 1, words.end());
	// std::cout << "size=" << this->param_list.size() << "\n";
	// check when no pwd --> A mettre dans une autre fct ?
	if (this->_command != "CAP" && this->_command != "PASS" && this->_nick == "" && this->_status != "connected/registered")
	{
		std::cout << RED "FAILED TO CONNECT : no password" E << std::endl;
		exit(0); // Quitter de maniere plus propre
	}

	// std::cout << "WORDS are:" << std::endl;
	// for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	// 	std::cout << ' ' << *it;
	// std::cout << std::endl;

	// std::cout << "PARAM LIST 1 are:" << std::endl;
	// for (std::vector<std::string>::iterator it = this->param_list.begin(); it != this->param_list.end(); ++it)
	// 	std::cout << ' ' << *it;
	// std::cout << std::endl;

	this->exec_cmd();
	this->clear_cmd();
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
		return;
	}
	else
	{
		std::cout << "exe>" << this->_command << "\n";
		this->_cmd_list[this->_command](this);
		this->update_activity();
	}
}

void User::update_activity(void)
{
	this->_last_activity = time(NULL);
}

const std::string&	User::getAway() const
{
	return awayMsg;
}
