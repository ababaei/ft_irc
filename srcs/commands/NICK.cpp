#include "../../inc/command.hpp"
#include "../../inc/colors.hpp"
#include "../../inc/User.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

// patern :   nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

int specialchar_nick(char c)
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

int check_forbiden_char_nick(std::string nick)
{
    if (nick != "")
    {
        unsigned int i = 0;
        if (nick.length() != 1 && nick[i] == ':') // tbc
        {
            i++;
        }
        if (nick.length() == 1 && isalpha(nick[i]) == 0 && specialchar_nick(nick[i]) == 0)
            return (-1);
        while (i < nick.length() - 1)
        {
            if ((isalpha(nick[i]) == 0 && specialchar_nick(nick[i]) == 0) || (nick.length() == 1 && nick[i] == ':'))
                return (-1);
            i++;
        }
        std::cout << "CHAR IS " << nick[i] << std::endl;
        if (isalnum(nick[i]) == 0 && specialchar_nick(nick[i]) == 0)
            return (-1);
        return (0);
    }
    return (0);
}

void NICK(User *user)
{
    if (user->param_list[0][0] == ':') // tbc
        user->param_list[0].erase(0, 1);
    std::string nickname = user->param_list[0].substr(0, 9);
    // user->param_list[0] = user->param_list[0].substr(0, 9);
    if (user->param_list.size() == 0)                                    // quand on passe NICK dans irssi sans param ca fait rien a part redire le nick
            return (user->getServer()->toSend(ERR_NONICKNAMEGIVEN(user->param_list, nickname), user->getFd()));

    if (check_forbiden_char_nick(nickname) == -1)
            return (user->getServer()->toSend(ERR_ERRONEUSNICKNAME(user->param_list, nickname), user->getFd()));
    std::map<int, User *> users = user->getServer()->getUserList();
    for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (nickname == it->second->getNick())
            return (user->getServer()->toSend(ERR_NICKNAMEINUSE(user->param_list, nickname), user->getFd()));
    }
    user->setNick(nickname);
}
