#ifndef SERVER_HPP
#define SERVER_HPP

#include "std.hpp"
#include "User.hpp"
#include "Channel.hpp"

#define PORT "6667"
#define DEBUG

class User;
class Channel;

class Server
{

public:
	Server(std::string arg_port, std::string pword);
	~Server(void);

	Server &operator=(Server const &rhs);

	void set_listener_sock(void); // set le socket listener
	void add_socket_to_list(int filed, short ev, short rev);
	void add_channel(std::string new_channel); // doublon avec arthur ?
	void add_nick(std::string new_nick);
	void remove_nick(std::string old_nick);
	void deleteUser(const std::string& nick);
	void deleteChannel(const std::string& name);
	void add_channel(std::string new_channel, Channel *chan); // doublon avec arthur ?
	void handle_pfds(void);
	void handle_new_connection(void);
	void close_connection(int sender_fd, int nbytes);
	void handle_raw(int sender_fd);

	void arr_to_list(void);
	void list_to_arr(void);
	void poll_loop(void);
	void polling(void);

	void to_send(const std::string msg, std::vector<int> sockfds);
	void to_send(const std::string msg, int sockfd);

	void check_activity(void);
	bool isHere(const std::string &nick); // checks if a User is present in the server

	/* accessors */
	std::string get_address(void);
	std::string get_password(void);
	std::string get_server_name(void);
	std::map<int, User *> get_user_list(void);
	Channel *get_channel(const std::string &name);
	User *get_user(const std::string &nick);
	std::map<std::string, Channel *> get_channel_list(void);

private:
	std::string _message;
	std::string _port;
	std::string _password;
	std::string _address;
	std::string _server_name;

	std::list<pollfd> _pfds;
	struct pollfd *_arr_pfds;

	std::map<int, User *> _User_list;
	std::map<std::string, Channel *> _channels;

	char _buf[512];
	int _listener;
};

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif
