#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <iostream>
# include "User.hpp"
# include "Server.hpp"

void PASS(User *User);
void NICK(User *User);
void USER(User *User);
void MODE(User *User);
void WHOIS(User *User);
void PING(User *User);




#endif