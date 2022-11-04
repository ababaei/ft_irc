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

void LIST(User *user)
{
    std::cout << "Hello LIST" << std::endl;
    // if (List tout seul)
    //     affiche all channels (titre + topic)
    // if (list + un ou plusieurs channel en param )
    //     affiche titre + topic du/des channel

    std::map<std::string, Channel *> channelList = user->get_server()->get_channel_list();
    if (user->param_list.size() == 0)
    {
        std::cout << YELLOW "All channels and their topic" E << std::endl;
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
            std::cout << YELLOW << it->first << "   " << it->second->getTopic() << E << std::endl;
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
                std::cout << GREEN << user->get_server()->get_channel(token)->getName() << ' ' << user->get_server()->get_channel(token)->getTopic() << E << std::endl;
            }
            s.erase(0, pos + delimiter.length());
        }
        if (channelList.count(s) == 1)
        {
                std::cout << GREEN << user->get_server()->get_channel(s)->getName() << ' ' << user->get_server()->get_channel(s)->getTopic() << E << std::endl;
        }
    }
}
