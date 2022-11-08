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
// ERR_USERONCHANNEL = <user> <channel> :is already on channel
// ERR_CHANOPRIVSNEEDED = <channel> :You're not channel operator
// RPL_INVITING = <channel> <nick>" Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client.
// RPL_AWAY = "<nick> :<away message>"

bool channel_exists(User *user)
{
    std::map<std::string, Channel *> list = user->get_server()->get_channel_list();

    for (std::map<std::string, Channel *>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it->second->getName() == user->param_list[0])
            return true;
        return false;
    }
    return false;
}

bool in_channel(User *user, std::string person)
{
    std::vector<std::string> list = user->get_server()->get_channel(user->param_list[0])->getNickList();
    if (std::find(list.begin(), list.end(), person) != list.end())
        return true;
    return false;
}

bool nick_exists(User *user)
{
    std::map<int, User *> list = user->get_server()->get_user_list();
    for (std::map<int, User *>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it->second->get_nick() == user->param_list[1])
            return true;
        return false;
    }
    return false;
}

void INVITE(User *user)
{
    if (user->param_list.size() < 2)
        std::cout << RED "ERR_NEEDMOREPARAMS" E << std::endl; // rajouter reply

    // chan en 1er
    if (user->param_list[0][0] == '#')
    {

        if (nick_exists(user) == false)
            std::cout << RED "ERR_NOSUCHNICK" E << std::endl; // rajouter reply
        if (channel_exists(user) == false)
        {
            std::cout << RED "Channel does not exist" E << std::endl; // rajouter reply
            return;
        }
        if (in_channel(user, user->get_nick()) == false)
            std::cout << RED "ERR_NOTONCHANNEL you're not on that channel" E << std::endl; // rajouter reply
        if (in_channel(user, user->param_list[1]) == true)
            std::cout << RED "ERR_USERONCHANNEL is already on that channel" E << std::endl; // rajouter reply


    }
    else if (user->param_list[0][0] == '#')
    {
    }
}
