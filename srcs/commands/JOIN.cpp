#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"
#include "../../inc/Channel.hpp"

#include <iostream>
 #include <algorithm>

void JOIN(User *user)
{
    // Pattern : ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
    //    / "0"
    // #chan
    // #chan key
    // #chan, #chan
    // #chan, #chan key, key
    // #chan, #chan key

    //     +pareil avec &instead of #


    // check debut is # ou & quand cest pas une key
    // check needmore param s'il faut une key (ERR_NEEDMOREPARAMS)
    // Key correct (ERR_BADCHANNELKEY)
    // check que cest pas sur invite (ERR_INVITEONLYCHAN)
    // Channel names may contain a maximum of 164 characters. This limit includes channel prefixes (i.e. private- and presence- are included in the character count). ERR_NOSUCHCHANNEL
    // Channel names should only include lower and uppercase letters, numbers and the following punctuation _ - = @ , . ; ERR_NOSUCHCHANNEL
    // Cannot join chan is banned (ERR_BANNEDFROMCHAN)
    // ERR_CHANNELISFULL (500)
    // joined too many chans : ERR_TOOMANYCHANNELS
    // ERR_TOOMANYTARGETS : Returned to a client which is attempting to JOIN a safe
    //        channel using the shortname when there are more than one
    //        such channel. --> Necessaire ?
    // ERR_BADCHANMASK --> Necessaire ?
    // ERR_UNAVAILRESOURCE ?
    // JOIN 0 = leave all channel grace via PART
    int j = 0;
    while (j <= user->param_list.size())
    {
        std::cout << "params JOINS are: " << user->param_list[j] << std::endl;
        j++;
    }

    int i = 0;
    std::string chan1;
    std::string chan2;
    std::string pwdchan1;
    std::string pwdchan2;

    //nom chan 1
    if (user->param_list[0][0] != '#' && user->param_list[0][0] != '&')
    {
        std::cout << RED "Wrong syntaxe" E << std::endl;
    }
    std::string delimiter = ",";//suivi d'espace ou pas ?
    if (user->param_list[0][0] == '#' || user->param_list[0][0] == '&')
    {
        chan1 = user->param_list[0].substr(0, user->param_list[0].find(delimiter)).erase(0,1);
        chan2 = user->param_list[0].substr(user->param_list[0].find(delimiter) + 1, user->param_list[0].size()).erase(0,1);
    }

    if (user->param_list[1][0] != '#' && user->param_list[1][0] != '&')
    {
        pwdchan1 = user->param_list[0].substr(0, user->param_list[0].find(delimiter));
        pwdchan2 = user->param_list[0].substr(user->param_list[0].find(delimiter) + 1, user->param_list[0].size());
    }

    std::cout << "NOM CHAN 1: " << chan1 << std::endl;
    std::cout << "NOM CHAN 2: " << chan2 << std::endl;
    std::cout << "NOM PWDCHAN 1: " << pwdchan1 << std::endl;
    std::cout << "NOM PWDCHAN 2: " << pwdchan2 << std::endl;

    

    std::vector<std::string> channel_list = user->get_server()->get_channels_list();
    if (std::find(channel_list.begin(), channel_list.end(), chan1) == channel_list.end())
    {
        // creation du chan
       user->get_server()->add_channel(chan1); // ?? 
       Channel *chan = new Channel(chan1);
       if (pwdchan1 != "")
         chan->setKey(pwdchan1);
        user->add_channel(chan1);
        // add operator mode for this chan for this user
    }
    else 
    {
        //check mdp s'il y a 
        
        //Joindre le chan
    }
    // if (user->param_list[0] == "0")
    // {
    //     // Part(all_channels);
    // }
    // else
    // {
    //     int i = 0;
    //     while (i < user->param_list.size())
    //     {
    //         user->param_list[i].erase(0, 1);
    //         std::vector<std::string> channel_list = user->get_server()->get_channels_list();
    //         if (std::find(channel_list.begin(), channel_list.end(), user->param_list[i]) == channel_list.end())
    //         {
    //             //creation du chan
    //             user->get_server()->add_channel(user->param_list[i]);
    //             // add operator mode for this chan for this user
    //         }
    //         else
    //         {
    //             // joindre le chan deja cree
    //             // RPL_TOPIC
    //             // RPL_NAMREPLY (353)
    //             // RPL_ENDOFNAMES (366).
    //         }
    //         i++;
    //     }
    // }

    std::vector<std::string> channel_list2 = user->get_server()->get_channels_list();
    std::cout << "Now list chan is" << std::endl;
    for (std::vector<std::string>::iterator it = channel_list2.begin(); it != channel_list2.end(); ++it)
        std::cout << " " << *it;
}