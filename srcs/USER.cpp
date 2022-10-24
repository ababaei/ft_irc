#include "../inc/command.hpp"

void    USER(User *User)
{
    std::string msg = ":127.0.0.1 001 " + User->get_nick() + "!ababaei@127.0.0.1\r\n"; 
    std::cout << "THIS IS INSIDE USER CMD\n";
    send(User->get_fd(), msg.c_str(), msg.length(), 0);
}