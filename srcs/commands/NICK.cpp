#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"
#include "../../inc/User.hpp"
#include "../../inc/Server.hpp"
#include <iostream>
#include <map>
#include <string>

// patern :   nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

int specialchar(char c)
{
    if (c == '-' ||
        c == '[' ||
        c == ']' ||
        c == '\\' ||
        c == '`' ||
        c == '_' ||
        c == '^' ||
        c == '{' ||
        c == '|' ||
        c == '}')
        return (1);
    return (0);
}

// patern :   nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

int check_forbiden_char(std::string nick)
{
    if (nick != "")
    {
        int i = 0;
        if (nick.length() != 1 && nick[i] == ':') // tbc
            i++;
        if (nick.length() == 1 && isalpha(nick[i]) == 0 && specialchar(nick[i]) == 0)
            return (-1);
        while (i < nick.length() - 1)
        {
            if (isalpha(nick[i]) == 0 && specialchar(nick[i]) == 0 || (nick.length() == 1 && nick[i] == ':'))
                return (-1);
            i++;
        }
        if (isalnum(nick[i]) == 0 && specialchar(nick[i]) == 0)
            return (-1);
        return (0);
    }
    return (0);
}

void NICK(User *User)
{
    if (User->param_list[0][0] == ':') // tbc
        User->param_list[0].erase(0, 1);
    User->param_list[0] = User->param_list[0].substr(0, 8);
    if (User->param_list[0] == "")                                       // quand on passe NICK dans irssi sans param ca fait rien a part redire le nick
        std::cout << RED "Error 431 ERR_NONICKNAMEGIVEN" E << std::endl; // ajouter reply
    if (check_forbiden_char(User->param_list[0]) == -1)
        std::cout << RED "Error 432 ERR_ERRONEUSNICKNAME" E << std::endl; // ajouter reply
    else
    {
        User->set_nick(User->param_list[0]);
    }

    // 437    ERR_UNAVAILRESOURCE ?
    // 433    ERR_NICKNAMEINUSE : User is not a valid type
    // 484    ERR_RESTRICTED ?? : Sent by the server to a user upon connection to indicate
    //    the restricted nature of the connection (user mode "+r").
    std::cout << BBLUE "Your nick is: " << User->get_nick() << E << std::endl;
}