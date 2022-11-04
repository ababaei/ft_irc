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
                std::cout << GREEN << "name is: " << token << E << std::endl;
                std::cout << GREEN << "name is(with user): " << user->get_server()->get_channel(token)->getName() << E << std::endl;
                std::cout << GREEN << "topic is(with user): " << user->get_server()->get_channel(token)->getTopic() << E << std::endl;
                std::cout << GREEN << token << E << std::endl;
            }
            s.erase(0, pos + delimiter.length());
        }
        if (channelList.count(s) == 1)
        {
            std::cout << YELLOW << "name is: " << s << E << std::endl;
            std::cout << YELLOW << "name is(with user): " << user->get_server()->get_channel(s)->getName() << E << std::endl;
            std::cout << YELLOW << "topic is(with user): " << user->get_server()->get_channel(s)->getTopic() << E << std::endl;
            std::cout << YELLOW << s << E << std::endl;
        }

        // std::string delimiter = ","; // suivi d'espace ou pas ?
        // int i = 0;
        // for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        // {
        //     std::string param = user->param_list[0];
        //     std::string delimiter = ",";

        //     size_t pos = 0;
        //     std::string token;
        //     while ((pos = param.find(delimiter)) != std::string::npos)
        //     {
        //         token = param.substr(0, pos);
        //         if (token == it->first)
        //         {
        //             std::cout << GREEN "the name is 1: " << it->second->getName() << " the topic is: " << it->second->getTopic() << E << std::endl;
        //             std::cout << GREEN << token << E << std::endl;
        //         }
        //         param.erase(0, pos + delimiter.length());
        //     }
        //     if (param == it->second->getName())
        //     {
        //         // std::cout << CYAN "the name is 2: " << it->second->getName() << " the topic is: " << it->second->getTopic() << E << std::endl;
        //         std::cout << CYAN "the name is 2: " << param << " the topic is: " << user->get_server()->get_channel(param)->getTopic() << E << std::endl;
        //         std::cout << CYAN << param << E << std::endl;
        //     }
        // }
        // std::string param = user->param_list[0].substr(0, user->param_list[0].find(delimiter));
        // std::cout << GREEN << it->first << "   " << it->second->getTopic() << E << std::endl;
        // if (param == it->second->getName())
        //     std::cout << GREEN "the name is: " << it->second->getName() << " the topic is: " << it->second->getTopic() << E << std::endl;
    }

    std::cout << RED "List params" << std::endl;
    for (std::vector<std::string>::iterator it = user->param_list.begin(); it != user->param_list.end(); it++)
        std::cout << ' ' << E << *it;
    std::cout << std::endl;

    // std::cout << "List channels" << std::endl;
    // std::map<std::string, Channel *> channels = user->get_server()->get_channel_list();
    // for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    //     std::cout << it->first << "   " << it->second->getTopic() << std::endl;

    // std::cout << "List topics" << std::endl;
    // for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    //     const std::string sujet = user->get_server()->get_channel(it->second)->getTopic();
}
