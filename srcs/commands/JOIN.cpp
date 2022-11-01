#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"
#include "../../inc/Channel.hpp"

#include <iostream>
#include <algorithm>

int specialchar_join(char c)
{
    // _ - = @ , . ;
    if (c == '_' ||
        c == '-' ||
        c == '=' ||
        c == '@' ||
        c == ',' ||
        c == '.' ||
        c == ';')
        return (1);
    return (0);
}

int check_forbiden_char_join(std::string channel)
{
    if (channel != "")
    {
        int i = 0;
        while (i < channel.length())
        {
            if (isalnum(channel[i]) == 0 && specialchar_join(channel[i]) == 0)
                return (-1);
            i++;
        }
    }
    return (0);
}

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

    // Il faudrait que dans JOIN, tu n'oublies pas d'ajouter le channel au server et le user au channel.

    // check debut is # ou & quand cest pas une key
    // check needmore param s'il faut une key (ERR_NEEDMOREPARAMS)
    // Key correct (ERR_BADCHANNELKEY)
    // check que cest pas sur invite (ERR_INVITEONLYCHAN)
    // OK chan lenght = 164 max ERR_NOSUCHCHANNEL
    // OK Channel names should only include lower and uppercase letters, numbers and the following punctuation _ - = @ , . ; ERR_NOSUCHCHANNEL
    // OK Cannot join chan is banned (ERR_BANNEDFROMCHAN)
    // OK ERR_CHANNELISFULL
    // joined too many chans : ERR_TOOMANYCHANNELS
    // ERR_TOOMANYTARGETS : Returned to a client which is attempting to JOIN a safe
    //        channel using the shortname when there are more than one
    //        such channel. --> Necessaire ?
    // ERR_BADCHANMASK --> Necessaire ?
    // ERR_UNAVAILRESOURCE ?
    // JOIN 0 = leave all channel grace via PART
    std::cout << "PARAM LIST 2 are:" << std::endl;
    for (std::vector<std::string>::iterator it = user->param_list.begin(); it != user->param_list.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << ITGREY " || size is: " << user->param_list.size() /* << " || capacity is: " << a_vector.capacity() */ << E;

    std::cout << std::endl;

    int i = 0;
    std::string chan1;
    std::string chan2;
    std::string pwdchan1;
    std::string pwdchan2;

    if (user->param_list[0][0] != '#' && user->param_list[0][0] != '&')
    {
        std::cout << RED "Wrong syntaxe" E << std::endl;
    }
    std::string delimiter = ","; // suivi d'espace ou pas ?
    if (user->param_list[0][0] == '#' || user->param_list[0][0] == '&')
    {
        chan1 = user->param_list[0].substr(0, user->param_list[0].find(delimiter)).erase(0, 1);
        if (chan1.size() + 1 != user->param_list[0].size())
            chan2 = user->param_list[0].substr(user->param_list[0].find(delimiter) + 1, user->param_list[0].size()).erase(0, 1);
    }
    if (user->param_list.size() != 1 && user->param_list[1][0] != '#' && user->param_list[1][0] != '&' && isalnum(user->param_list[1][0]) != 0)
    {
        pwdchan1 = user->param_list[1].substr(0, user->param_list[1].find(delimiter));
        if (pwdchan1.size() != user->param_list[1].size())
            pwdchan2 = user->param_list[1].substr(user->param_list[1].find(delimiter) + 1, user->param_list[1].size());
    }

    std::cout << "NOM CHAN 1: " << chan1 << std::endl;
    std::cout << "NOM CHAN 2: " << chan2 << std::endl;
    std::cout << "NOM PWDCHAN 1: " << pwdchan1 << std::endl;
    std::cout << "NOM PWDCHAN 2: " << pwdchan2 << std::endl;

    // faire aussi pour chan 2
    std::map<std::string, Channel *> chans = user->get_server()->get_channel_list();
    if (chans.empty() == 1)
    {
        std::cout << "Creating the chan 1" << std::endl;
        if (chan1.size() > 164)
            std::cout << RED "403 ERR_NOSUCHCHANNEL" E << std::endl; // ajouter reply
        if (check_forbiden_char_join(chan1) == -1)
            std::cout << RED "Error 432 ERR_ERRONEUSNICKNAME" E << std::endl; // ajouter reply
        Channel *chan = new Channel(chan1);
        chan->setName(chan1);
        chan->addUser(user);
        user->get_server()->add_channel(chan1, chan);
        user->add_channel(chan1);
        if (pwdchan1 != "")
            chan->setKey(pwdchan1);
    }

    for (std::map<std::string, Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
    {
        std::cout << "COMPAREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;

        if (chan1 == it->second->getName())
        {
            // check mdp s'il y a
            if (it->second->isBanned(user->get_nick()) == 1)
                std::cout << RED "Error ERR_BANNEDFROMCHAN" E << std::endl; // ajouter reply
            if (it->second->getUserList().size() >= it->second->getUserLimit())
                std::cout << RED "Error 432 ERR_CHANNELISFULL" E << std::endl; // ajouter reply
            // if ()
            // ERR_TOOMANYCHANNELS

            std::cout << "Joined the chan" << std::endl;
            //             // RPL_TOPIC
            //             // RPL_NAMREPLY (353)
            //             // RPL_ENDOFNAMES (366).
        }
        else
        {
            std::cout << "Creating the chan 1" << std::endl;
            if (chan1.size() > 164)
                std::cout << RED "403 ERR_NOSUCHCHANNEL" E << std::endl; // ajouter reply
            if (check_forbiden_char_join(chan1) == -1)
                std::cout << RED "Error 432 ERR_ERRONEUSNICKNAME" E << std::endl; // ajouter reply
            Channel *chan = new Channel(chan1);
            chan->setName(chan1);
            chan->addUser(user);
            user->get_server()->add_channel(chan1, chan);
            user->add_channel(chan1);
            if (pwdchan1 != "")
                chan->setKey(pwdchan1);
        }
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

    // std::vector<std::string> channel_list2 = user->get_server()->get_channels_list();
    // std::cout << "Now list chan is" << std::endl;
    // for (std::vector<std::string>::iterator it = channel_list2.begin(); it != channel_list2.end(); ++it)
    //     std::cout << " " << *it;
}
