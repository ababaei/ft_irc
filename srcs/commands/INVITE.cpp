#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"
#include "../../inc/colors.hpp"

//    Parameters: <nickname> <channel>

// if the channel exists, only members of the channel
//    are allowed to invite other users.  When the channel has invite-only
//    flag set, only channel operators may issue INVITE command.

// Only the user inviting and the user being invited will receive
//    notification of the invitation.

// OK ERR_NEEDMOREPARAMS = manque param
// OK ERR_NOSUCHNICK = No such nick/channel
// OK ERR_NOTONCHANNEL = You're not on that channel
// OK ERR_USERONCHANNEL = <user> <channel> :is already on channel
// OK ERR_CHANOPRIVSNEEDED = <channel> :You're not channel operator
// OK RPL_INVITING = <channel> <nick>" Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client.
// OK RPL_AWAY = "<nick> :<away message>"

bool channel_exists(User *user, std::string channel)
{
    std::map<std::string, Channel *> list = user->get_server()->get_channel_list();

    for (std::map<std::string, Channel *>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it->second->getName() == channel)
            return true;
        return false;
    }
    return false;
}

bool in_channel(User *user, std::string person, std::string channel)
{
    std::vector<std::string> list = user->get_server()->get_channel(channel)->getNickList();
    if (std::find(list.begin(), list.end(), person) != list.end())
        return true;
    return false;
}

bool nick_exists(User *user, std::string nick)
{
    std::map<int, User *> list = user->get_server()->get_user_list();
    for (std::map<int, User *>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it->second->get_nick() == nick)
            return true;
        return false;
    }
    return false;
}

bool is_operator(User *user, std::string channel)
{
    if (user->get_server()->get_channel(channel)->isInviteOnly() == 0)
        return true;
    if (user->get_server()->get_channel(channel)->isChanOp(user->get_nick()) == 1)
        return true;
    return (false);
}

bool is_away(User *user)
{
    if (user->get_status() == "away")
        return (true);
    return (false);
}

void INVITE(User *user)
{
    if (user->param_list.size() < 2)
    {
        std::cout << RED "ERR_NEEDMOREPARAMS" E << std::endl; // rajouter reply
        return;
    }

    // chan en 1er
    if (user->param_list[0][0] == '#')
    {
        if (nick_exists(user, user->param_list[1]) == false)
        {
            std::cout << RED "ERR_NOSUCHNICK" E << std::endl; // rajouter reply
            return;
        }
        if (channel_exists(user, user->param_list[0]) == false)
        {
            std::cout << RED "Channel does not exist" E << std::endl; // rajouter reply
            return;
        }
        if (in_channel(user, user->get_nick(), user->param_list[0]) == false)
        {
            std::cout << RED "ERR_NOTONCHANNEL you're not on that channel" E << std::endl; // rajouter reply
            return;
        }
        if (in_channel(user, user->param_list[1], user->param_list[0]) == true)
        {
            std::cout << RED "ERR_USERONCHANNEL is already on that channel" E << std::endl; // rajouter reply
            return;
        }
        if (is_operator(user, user->param_list[0]) == false)
        {
            std::cout << RED "ERR_CHANOPRIVSNEEDED is invite only and you're not ops" E << std::endl; // rajouter reply.
            return;
        }
        if (is_away(user) == true)
            user->get_server()->to_send(RPL_AWAY(getArgs(user->get_nick(), "is away from keyboard."), user->get_nick()), user->get_fd());
        // std::cout << RED "is away" E << std::endl; // rajouter reply.
        std::cout << GREEN "RPL_INVITING is invited" E << std::endl; // rajouter reply.
    }
    else if (user->param_list[1][0] == '#')
    {
        if (nick_exists(user, user->param_list[0]) == false)
        {
            std::cout << RED "ERR_NOSUCHNICK" E << std::endl; // rajouter reply
            return;
        }
        if (channel_exists(user, user->param_list[1]) == false)
        {
            std::cout << RED "Channel does not exist" E << std::endl; // rajouter reply
            return;
        }
        if (in_channel(user, user->get_nick(), user->param_list[1]) == false)
        {
            std::cout << RED "ERR_NOTONCHANNEL you're not on that channel" E << std::endl; // rajouter reply
            return;
        }
        if (in_channel(user, user->param_list[0], user->param_list[1]) == true)
        {
            std::cout << RED "ERR_USERONCHANNEL is already on that channel" E << std::endl; // rajouter reply
            return;
        }
        if (is_operator(user, user->param_list[1]) == false)
        {
            std::cout << RED "ERR_CHANOPRIVSNEEDED is invite only and you're not ops" E << std::endl; // rajouter reply.
            return;
        }
        if (is_away(user) == true)
            user->get_server()->to_send(RPL_AWAY(getArgs(user->get_nick(), "is away from keyboard."), user->get_nick()), user->get_fd());
        // std::cout << RED "is away" E << std::endl; // rajouter reply.
        std::cout << GREEN "RPL_INVITING is invited" E << std::endl; // rajouter reply.
    }
}
