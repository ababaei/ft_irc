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

//     +pareil avec & + !

int specialchar_join(char c)
{
    // _ - = @ , . ;

    //(' '), a control G (^G or ASCII 7), ',' , ':'

    if (c == ' ' ||
        c == 7 ||
        c == ',' ||
        c == ':')
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
            if (specialchar_join(channel[i]) == 1)
                return (0);
            i++;
        }
    }
    return (1);
}

void create_channel(User *user, std::string channel, std::string pwdchan)
{
    std::cout << "Creating the chan" << std::endl;
    if (channel.size() >= 50)
        return user->get_server()->to_send(ERR_NOSUCHCHANNEL(getArgs(channel), user->get_nick()),
                                           user->get_fd());
    if (check_forbiden_char_join(channel) == 0)
        return user->get_server()->to_send(ERR_BADCHANMASK(getArgs(channel), user->get_nick()),
                                           user->get_fd());

    Channel *chan = new Channel(channel);
    chan->setName(channel);
    chan->addUser(user);
    user->get_server()->add_channel(channel, chan);
    user->add_channel(channel);
    user->set_mode("operator", 1);
    YELLOW;
    user->get_server()->to_send(getMsg(user, "JOIN", channel), user->get_fd()); // ?? Est ce une reply

    if (pwdchan != "")
        chan->setKey(pwdchan);

    std::string reply_channel;
    std::string reply_nick;

    if (chan->isSecret() == true)
        reply_channel = "@" + channel;
    else if (chan->isPrivate() == true)
        reply_channel = "*" + channel;
    else
        reply_channel = "=" + channel;

    std::vector<std::string> nick_list = chan->getNickList();
    for (std::vector<std::string>::iterator it = nick_list.begin(); it != nick_list.end(); ++it)
    {
        std::string tmp;
        if (chan->isChanOp(*it))
            tmp = "@" + *it;
        else if (chan->canSpeak(*it))
            tmp = "+" + *it;
        reply_nick += tmp;
        std::cout << ' ' << *it;
    }
    return user->get_server()->to_send(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
                                                     user->get_nick()),
                                       user->get_fd());
    // return user->get_server()->to_send(RPL_ENDOFNAMES(getArgs(channel),
    //                                                   user->get_nick()),
    //                                    user->get_fd());
}

void join_channel(Channel *chan, User *user)
{
    std::string channel = chan->getName();
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
    std::string reply_channel;
    std::string reply_nick;

    if (chan->isSecret() == true)
        reply_channel = "@" + channel;
    else if (chan->isPrivate() == true)
        reply_channel = "*" + channel;
    else
        reply_channel = "=" + channel;

    std::vector<std::string> nick_list = chan->getNickList();
    for (std::vector<std::string>::iterator it = nick_list.begin(); it != nick_list.end(); ++it)
    {
        std::string tmp;
        if (chan->isChanOp(*it))
            tmp = "@" + *it;
        else if (chan->canSpeak(*it))
            tmp = "+" + *it;
        reply_nick += tmp;
        std::cout << ' ' << *it;
    }
    return user->get_server()->to_send(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
                                                     user->get_nick()),
                                       user->get_fd());
    // return user->get_server()->to_send(RPL_ENDOFNAMES(getArgs(channel),
    //                                                   user->get_nick()),
    //                                    user->get_fd());
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
    std::map<std::string, Channel *> channelList = user->get_server()->get_channel_list();
    if (user->param_list[0] == "0" || user->param_list[0] == "#0")
    {
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            std::cout << RED "Part " << it->first << E << std::endl;
            user->to_command("PART " + it->first);
        }
        return;
    }

    std::string key;
    std::string delimiter = ","; // suivi d'espace ou pas ?
    std::vector<std::string> listNewChans;
    if (user->param_list[0][0] == '#' || user->param_list[0][0] == '&' || user->param_list[0][0] == '+' || user->param_list[0][0] == '!')
        listNewChans = splitStr(user->param_list[0], ",");

    std::vector<std::string> listNewPwd;
    if (user->param_list.size() != 1 && user->param_list[1][0] != '#' && user->param_list[1][0] != '&' && user->param_list[1][0] != '+' && user->param_list[1][0] != '!' && isalnum(user->param_list[1][0]) != 0)
    {
        if (user->param_list[1][0] != '#' || user->param_list[1][0] != '&' || user->param_list[1][0] != '+')
            listNewPwd = splitStr(user->param_list[1], ",");
    }

    std::cout << YELLOW "chans are:" E << std::endl;
    for (std::vector<std::string>::iterator it = listNewChans.begin(); it != listNewChans.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;

    std::cout << YELLOW "pwd are:" E << std::endl;
    for (std::vector<std::string>::iterator it = listNewPwd.begin(); it != listNewPwd.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;

    unsigned int i = 0;
    while (i < listNewChans.size())
    {
        Channel *chan = user->get_server()->get_channel(listNewChans[i]);
        if (listNewPwd.size() > 0)
        {
            if (listNewPwd[i].size() > 0)
                key = listNewPwd[i];
        }
        else
        {
            key = "";
        }
        if (chan == NULL)
        {
            create_channel(user, listNewChans[i], key);
        }
        else
        {
            if (key == chan->getKey())
                join_channel(chan, user);
            else
                user->get_server()->to_send(ERR_BADCHANNELKEY(getArgs(listNewChans[i]), user->get_nick()),
                                            user->get_fd());
        }
        i++;
    }

    std::cout << YELLOW "all chan + pwd + nick in the chan" E << std::endl;
    std::map<std::string, Channel *> channelsList = user->get_server()->get_channel_list();
    for (std::map<std::string, Channel *>::iterator it = channelsList.begin(); it != channelsList.end(); ++it)
    {
        std::cout << it->first << "   ";
        std::cout << it->second->getKey() << std::endl;
        std::vector<std::string> listNick = it->second->getNickList();
        for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
            std::cout << ' ' << *it << std::endl;
    }
}
