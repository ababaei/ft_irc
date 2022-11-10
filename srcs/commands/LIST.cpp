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

std::string itoa(int a)
{
    std::string ss = "";
    while (a)
    {
        int x = a % 10;
        a /= 10;
        char i = '0';
        i = i + x;
        ss = i + ss;
    }
    return ss;
}

void LIST(User *user)
{
    std::cout << "Hello LIST" << std::endl;
    // if (List tout seul)
    //     affiche all channels (titre + topic)
    // if (list + un ou plusieurs channel en param )
    //     affiche titre + topic du/des channel
    // reply : RPL_LIST :"<channel> <nombres> :<topic>"
    std::map<std::string, Channel *> channelList = user->get_server()->get_channel_list();
    if (user->param_list.size() == 0)
    {
        std::cout << YELLOW "All channels and their topic" E << std::endl;
        for (std::map<std::string, Channel *>::iterator itm = channelList.begin(); itm != channelList.end(); ++itm)
        {
            int nombre = 0;
            std::vector<std::string> listNick = itm->second->getNickList();
            for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
            {
                if (user->get_server()->get_user(*it)->get_mode("invisible") == false)
                    nombre++;
                std::cout << ' ' << *it;
            }
            user->get_server()->to_send(RPL_LIST(getArgs(itm->first, itoa(nombre), itm->second->getTopic()), user->get_nick()),
                                        user->get_fd());
            // std::cout << YELLOW << itm->first << "   " << nombre << itm->second->getTopic() << E << std::endl;
        }
    }
    else
    {
        std::string s = user->param_list[0];
        std::string delimiter = ",";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            token = s.substr(0, pos);
            if (channelList.count(token) == 1)
            {
                int nombre = 0;
                std::vector<std::string> listNick = user->get_server()->get_channel(token)->getNickList();
                for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
                {
                    if (user->get_server()->get_user(*it)->get_mode("invisible") == false)
                        nombre++;
                    std::cout << ' ' << *it;
                }
                user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(token)->getName(), itoa(nombre), user->get_server()->get_channel(token)->getTopic()), user->get_nick()),
                                            user->get_fd());
                // std::cout << GREEN << user->get_server()->get_channel(token)->getName() << ' ' << user->get_server()->get_channel(token)->getTopic() << E << std::endl;
            }
            s.erase(0, pos + delimiter.length());
        }
        if (channelList.count(s) == 1)
        {
            // std::cout << GREEN << user->get_server()->get_channel(s)->getName() << ' ' << user->get_server()->get_channel(s)->getTopic() << E << std::endl;
            int nombre = 0;
            std::vector<std::string> listNick = user->get_server()->get_channel(s)->getNickList();
            for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
            {
                if (user->get_server()->get_user(*it)->get_mode("invisible") == false)
                    nombre++;
                std::cout << ' ' << *it;
            }
            user->get_server()->to_send(RPL_LIST(getArgs(user->get_server()->get_channel(s)->getName(), itoa(nombre), user->get_server()->get_channel(s)->getTopic()), user->get_nick()),
                                        user->get_fd());
        }
    }
    return user->get_server()->to_send(RPL_LISTEND(getArgs(), user->get_nick()),
                                       user->get_fd());
}
