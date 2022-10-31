#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <iostream>
# include "User.hpp"
# include "Server.hpp"
# include "replies.hpp"
# include "utils.hpp"

void PASS(User *user);
void NICK(User *user);
void USER(User *user);
void MODE(User *user);
void JOIN(User *user);
void WHOIS(User *user);
void PING(User *user);




#endif
