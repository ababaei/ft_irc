#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"
#include "../../inc/utils.hpp"
#include "../../inc/colors.hpp"

//       Command: LIST
//    Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

//    The list command is used to list channels and their topics.  If the
//    <channel> parameter is used, only the status of that channel is
//    displayed.

//    If the <target> parameter is specified, the request is forwarded to
//    that server which will generate the reply. --> NO car 1 server

//    Wildcards are allowed in the <target> parameter. --> NO car 1 server

std::string itoa(int i)
{
    std::string str = "";
    if (i == 0)
        return ("0");
    while (i)
    {
        int x = i % 10;
        i /= 10;
        char c = '0';
        c = c + x;
        str = c + str;
    }
    return str;
}

std::string count_users(std::vector<std::string> listNick, User *user)
{
    int count = 0;
    for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
    {
        if (user->get_server()->get_user(*it)->get_mode("invisible") == false)
            count++;
        // std::cout << ' ' << *it;
    }
    return (itoa(count));
}

void LIST(User *user)
{
    // if (List tout seul)
    //     affiche all channels (titre + topic)
    // if (list + un ou plusieurs channel en param )
    //     affiche titre + topic du/des channel
    // reply : RPL_LIST :"<channel> <counts> :<topic>"
    std::map<std::string, Channel *> channelList = user->get_server()->get_channel_list();
    if (user->param_list.size() == 0)
    {
        // std::cout << YELLOW "All channels and their topic" E << std::endl;
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            std::vector<std::string> listNick = it->second->getNickList();
            user->get_server()->to_send(RPL_LIST(getArgs(it->first, count_users(listNick, user), it->second->getTopic()), user->get_nick()),
                                        user->get_fd());
            // std::cout << YELLOW << it->first << " " << finalcount << " " << it->second->getTopic() << E << std::endl;
        }
    }
    else
    {
        std::vector<std::string> channels = splitStr(user->param_list[0], ",");
        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            std::vector<std::string> listNick = user->get_server()->get_channel(*it)->getNickList();
            user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(*it)->getName(), count_users(listNick, user), user->get_server()->get_channel(*it)->getTopic()), user->get_nick()),
                                        user->get_fd());
        }
        std::string s = user->param_list[0];
        std::vector<std::string> listNick = user->get_server()->get_channel(s)->getNickList();
        user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(s)->getName(), count_users(listNick, user), user->get_server()->get_channel(s)->getTopic()), user->get_nick()),
                                    user->get_fd());

        // if (channels.size() == 0)
        // {
        // std::string s = user->param_list[0];
        // std::vector<std::string> listNick = user->get_server()->get_channel(s)->getNickList();
        // user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(s)->getName(), count_users(listNick, user), user->get_server()->get_channel(s)->getTopic()), user->get_nick()),
        //                             user->get_fd());
        // }

        // std::string s = user->param_list[0];
        // std::string delimiter = ",";
        // size_t pos = 0;
        // std::string token;
        // while ((pos = s.find(delimiter)) != std::string::npos)
        // {
        //     token = s.substr(0, pos);
        //     if (channelList.count(token) == 1)
        //     {
        //         std::vector<std::string> listNick = user->get_server()->get_channel(token)->getNickList();
        //         user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(token)->getName(), count_users(listNick, user), user->get_server()->get_channel(token)->getTopic()), user->get_nick()),
        //                                     user->get_fd());
        //         // std::cout << GREEN << user->get_server()->get_channel(token)->getName() << " " << itoa(count) << " " << user->get_server()->get_channel(token)->getTopic() << E << std::endl;
        //     }
        //     s.erase(0, pos + delimiter.length());
        // }
        // if (channelList.count(s) == 1)
        // {
        //     std::vector<std::string> listNick = user->get_server()->get_channel(s)->getNickList();
        //     user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(s)->getName(), count_users(listNick, user), user->get_server()->get_channel(s)->getTopic()), user->get_nick()),
        //                                 user->get_fd());
        //     // std::cout << CYAN << user->get_server()->get_channel(s)->getName() << " " << itoa(count) << " " << user->get_server()->get_channel(s)->getTopic() << E << std::endl;
        // }
    }
    return user->get_server()->to_send(RPL_LISTEND(getArgs(), user->get_nick()),
                                       user->get_fd());
}
