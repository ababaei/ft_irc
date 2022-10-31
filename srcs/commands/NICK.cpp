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

void NICK(User *user)
{
    std::string old_nick = user->get_nick();
    std::vector<std::string> nick_list = user->get_server()->get_nick_list();
    if (user->param_list[0][0] == ':') // tbc
        user->param_list[0].erase(0, 1);
    user->param_list[0] = user->param_list[0].substr(0, 9);
    if (user->param_list[0] == "")                                       // quand on passe NICK dans irssi sans param ca fait rien a part redire le nick
        std::cout << RED "Error 431 ERR_NONICKNAMEGIVEN" E << std::endl; // ajouter reply
    if (check_forbiden_char(user->param_list[0]) == -1)
        std::cout << RED "Error 432 ERR_ERRONEUSNICKNAME" E << std::endl; // ajouter reply
    if (user->param_list[0] == old_nick)
    {
        std::cout << YELLOW "This is already your NICK" E << std::endl; // utile ?
    }
    else if (std::count(nick_list.begin(), nick_list.end(), user->param_list[0]))
    {
        std::cout << RED "Error 433 ERR_NICKNAMEINUSE" E << std::endl; // ajouter reply
    }
    else
    {
        user->set_nick(user->param_list[0]);
        user->get_server()->add_nick(user->param_list[0]);
        user->get_server()->remove_nick(old_nick);
    }

    // 437    ERR_UNAVAILRESOURCE ?
    // 484    ERR_RESTRICTED ?? : Sent by the server to a user upon connection to indicate
    //    the restricted nature of the connection (user mode "+r").
    std::cout << BBLUE "Your nick is: " << user->get_nick() << E << std::endl;
}