#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/colors.hpp"
#include "../../inc/Channel.hpp"

#include <iostream>
#include <algorithm>

// Pattern : ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//    / "0"
// #chan
// #chan key
// #chan, #chan
// #chan, #chan key, key
// #chan, #chan key

//     +pareil avec &instead of #

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
        unsigned int i = 0;
        while (i < channel.length())
        {
            if (isalnum(channel[i]) == 0 && specialchar_join(channel[i]) == 0)
                return (-1);
            i++;
        }
    }
    return (0);
}

void create_chanel(User *user, std::string chanel, std::string pwdchan)
{
    std::cout << "Creating the chan" << std::endl;
    if (chanel.size() > 164)
        std::cout << RED "403 ERR_NOSUCHCHANNEL" E << std::endl; // ajouter reply
    if (check_forbiden_char_join(chanel) == -1)
        std::cout << RED "Error 432 ERR_ERRONEUSNICKNAME" E << std::endl; // ajouter reply
    Channel *chan = new Channel(chanel);
    chan->setName(chanel);
    chan->addUser(user);
    user->get_server()->add_channel(chanel, chan);
    user->add_channel(chanel);
    if (pwdchan != "")
        chan->setKey(pwdchan);
}

void join_chanel(std::map<std::string, Channel *>::iterator it, User *user)
{
    // check mdp s'il y a
    if (it->second->isBanned(user->get_nick()) == 1)
        std::cout << RED "Error ERR_BANNEDFROMCHAN" E << std::endl; // ajouter reply
    if (it->second->getUserList().size() >= static_cast<unsigned int>(it->second->getUserLimit()))
        std::cout << RED "Error 432 ERR_CHANNELISFULL" E << std::endl; // ajouter reply
    if (user->getChannelList().size() >= static_cast<unsigned int>(user->getChanelLimit()))
        std::cout << RED "Error ERR_TOOMANYCHANNELS" E << std::endl; // ajouter reply
    if (it->second->isInvited(user->get_nick()) == false && it->second->isInviteOnly() == 1)
        std::cout << RED "Error ERR_INVITEONLYCHAN" E << std::endl; // ajouter reply

    std::cout << "Joined the chan" << std::endl;
    //             // RPL_TOPIC // ajouter reply
    //             // RPL_NAMREPLY (353) // ajouter reply
    //             // RPL_ENDOFNAMES (366). // ajouter reply
}

void JOIN(User *user)
{
    // check needmore param s'il faut une key (ERR_NEEDMOREPARAMS)
    // Key correct (ERR_BADCHANNELKEY)
    // OK check que cest pas sur invite (ERR_INVITEONLYCHAN)
    // OK chan lenght = 164 max ERR_NOSUCHCHANNEL
    // OK Channel names should only include lower and uppercase letters, numbers and the following punctuation _ - = @ , . ; ERR_NOSUCHCHANNEL
    // OK Cannot join chan is banned (ERR_BANNEDFROMCHAN)
    // OK ERR_CHANNELISFULL
    // OK joined too many chans : ERR_TOOMANYCHANNELS

    // JOIN 0 = leave all channel grace via PART
    // if (user->param_list[0] == "0")
    // {
    //     // Part(all_channels);
    // }

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

    std::map<std::string, Channel *> chans = user->get_server()->get_channel_list();
    if (chans.empty() == 1)
    {
        create_chanel(user, chan1, pwdchan1);
    }

    for (std::map<std::string, Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
    {
        if (chan1 == it->second->getName())
            join_chanel(it, user);
        else
            create_chanel(user, chan1, pwdchan1);

        if (chan2 != "")
        {
            if (chan2 == it->second->getName())
                join_chanel(it, user);
            else
                create_chanel(user, chan2, pwdchan2);
        }
    }
    std::cout << "NOM CHAN 1: " << chan1 << std::endl;
    std::cout << "NOM CHAN 2: " << chan2 << std::endl;
    std::cout << "NOM PWDCHAN 1: " << pwdchan1 << std::endl;
    std::cout << "NOM PWDCHAN 2: " << pwdchan2 << std::endl;
}
