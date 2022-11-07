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
        unsigned int i = 1;
        while (i < channel.length())
        {
            if (isalnum(channel[i]) == 0 && specialchar_join(channel[i]) == 0)
                return (-1);
            i++;
        }
    }
    return (0);
}

void create_channel(User *user, std::string channel, std::string pwdchan)
{
    std::cout << "Creating the chan" << std::endl;
    if (channel.size() > 164)
        return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(channel), user->get_nick()),
                                           user->get_fd());
    if (check_forbiden_char_join(channel) == -1)
        return user->get_server()->to_send(ERR_BADCHANMASK(getArgs(channel), user->get_nick()),
                                           user->get_fd());

    Channel *chan = new Channel(channel);
    chan->setName(channel);
    chan->addUser(user);
    user->get_server()->add_channel(channel, chan);
    user->add_channel(channel);

    user->get_server()->to_send(getMsg(user, "JOIN", channel), user->get_fd());

    if (pwdchan != "")
    {
        std::cout << "PASSE 4" << std::endl;
        chan->setKey(pwdchan);
    }
}

void join_channel(Channel *chan, User *user)
{
    std::string channel = chan->getName();
    // check mdp s'il y a
    if (chan->isBanned(user->get_nick()) == 1)
        return user->get_server()->to_send(ERR_BANNEDFROMCHAN(getArgs(channel), user->get_nick()),
                                           user->get_fd());
    if (chan->getUserNum() >= chan->getUserLimit())
        return user->get_server()->to_send(ERR_CHANNELISFULL(getArgs(channel), user->get_nick()),
                                           user->get_fd());
    if (user->getChannelList().size() >= static_cast<unsigned int>(user->getChanelLimit()))
        return user->get_server()->to_send(ERR_TOOMANYCHANNELS(getArgs(channel), user->get_nick()),
                                           user->get_fd());
    if (chan->isInvited(user->get_nick()) == false && chan->isInviteOnly() == 1)
        return user->get_server()->to_send(ERR_INVITEONLYCHAN(getArgs(channel), user->get_nick()),
                                           user->get_fd());

    std::cout << "Joined the chan" << std::endl;
    user->get_server()->to_send(getMsg(user, "JOIN", channel),
                                chan->getFds());
    chan->addUser(user);
    user->add_channel(chan->getName());
    if (chan->getTopic() != "")
        user->get_server()->to_send(RPL_TOPIC(getArgs(channel, chan->getTopic()),
                                              user->get_nick()),
                                    user->get_fd());
    else
        user->get_server()->to_send(RPL_NOTOPIC(getArgs(channel),
                                                user->get_nick()),
                                    user->get_fd());

    //             // RPL_TOPIC // ajouter reply
    // OR RPL_NOTOPIC (331) if no topic is et //ajouter reply
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

    // if (user->param_list[0][0] != '#' && user->param_list[0][0] != '&')
    // {
    // std::cout << RED "Wrong syntaxe" E << std::endl;
    // }

    std::string delimiter = ","; // suivi d'espace ou pas ?
    std::vector<std::string> listNewChans;
    if (user->param_list[0][0] == '#' || user->param_list[0][0] == '&')
    {
        size_t pos = 0;
        std::string token;
        while ((pos = user->param_list[0].find(delimiter)) != std::string::npos)
        {
            token = user->param_list[0].substr(0, pos);
            // std::cout << token << std::endl;
            listNewChans.push_back(token);
            user->param_list[0].erase(0, pos + delimiter.length());
        }
        // std::cout << user->param_list[0] << std::endl;
        listNewChans.push_back(user->param_list[0]);
    }

    std::vector<std::string> listNewPwd;
    if (user->param_list.size() != 1 && user->param_list[1][0] != '#' && user->param_list[1][0] != '&' && isalnum(user->param_list[1][0]) != 0)
    {
        if (user->param_list[1][0] != '#' || user->param_list[1][0] != '&' || user->param_list[1][0] != '+')
        {
            size_t pos = 0;
            std::string token;
            while ((pos = user->param_list[1].find(delimiter)) != std::string::npos)
            {
                token = user->param_list[1].substr(0, pos);
                // std::cout << token << std::endl;
                listNewPwd.push_back(token);
                user->param_list[1].erase(0, pos + delimiter.length());
            }
            // std::cout << user->param_list[1] << std::endl;
            listNewPwd.push_back(user->param_list[1]);
        }
    }

    std::cout << YELLOW "chans are:" E << std::endl;
    for (std::vector<std::string>::iterator it = listNewChans.begin(); it != listNewChans.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;

    std::cout << YELLOW "pwd are:" E << std::endl;
    for (std::vector<std::string>::iterator it = listNewPwd.begin(); it != listNewPwd.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;

    // creations :
    unsigned int i = 0;
    while (i < listNewChans.size())
    {
        Channel *chan = user->get_server()->get_channel(listNewChans[i]);
        std::cout << "PASSE 1" << std::endl;
        if (listNewPwd.size() > 0)
        {
            if (listNewPwd[i].size() > 0)
                std::cout << "PASSE 5" << std::endl;
            pwdchan1 = listNewPwd[i];
        }
        else
        {
            std::cout << "PASSE 6" << std::endl;
            pwdchan1 = "";
        }
        std::cout << "PASSE 2" << std::endl;
        if (chan == NULL)
        {
            std::cout << "PASSE 3" << std::endl;
            create_channel(user, listNewChans[i], pwdchan1);
        }
        else
        {
            std::cout << "PASSE 4" << std::endl;
            if (pwdchan1 == chan->getKey())
                join_channel(chan, user);
            else
                user->get_server()->to_send(ERR_BADCHANNELKEY(getArgs(listNewChans[i]), user->get_nick()),
                                            user->get_fd());
        }
        i++;
    }
    // if (chan2 != "")
    // {
    //     Channel *chan_2 = user->get_server()->get_channel(chan2);
    //     if (chan == NULL)
    //         create_channel(user, chan2, pwdchan2);
    //     else
    //     {
    //         if (pwdchan1 == chan_2->getKey())
    //             join_channel(chan_2, user);
    //         else
    //             user->get_server()->to_send(ERR_BADCHANNELKEY(getArgs(chan2), user->get_nick()),
    //                                         user->get_fd());
    //     }
    // }
    ///////// fin creations

    std::cout << "NOM CHAN 1: " << chan1 << std::endl;
    std::cout << "NOM CHAN 2: " << chan2 << std::endl;
    std::cout << "NOM PWDCHAN 1: " << pwdchan1 << std::endl;
    std::cout << "NOM PWDCHAN 2: " << pwdchan2 << std::endl;

    std::cout << "all chan and pwd" << std::endl;
    std::map<std::string, Channel *> channelList = user->get_server()->get_channel_list();
    for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
    {
        std::cout << it->first << "   ";
        std::cout << it->second->getKey() << std::endl;
    }
}