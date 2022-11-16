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
        return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (check_forbiden_char_join(channel) == 0)
        return user->getServer()->toSend(ERR_BADCHANMASK(getArgs(channel), user->getNick()),
                                         user->getFd());

    Channel *chan = new Channel(channel);
    chan->setName(channel);
    chan->addUser(user);
    user->getServer()->addChannel(channel, chan);
    user->addChannel(channel);
    user->setMode("operator", 1);
    YELLOW;
    user->getServer()->toSend(getMsg(user, "JOIN", channel), user->getFd()); // ?? Est ce une reply
    user->getServer()->toSend(RPL_UNIQOPIS(getArgs(channel, user->getNick()),
                                           user->getNick()),
                              chan->getFds());

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
        // std::cout << ' ' << *it;
    }
    // a garder car dans la doc https://www.rfc-editor.org/rfc/rfc2812#section-3.2
    std::cout << "You have created and joined the channel " << reply_channel << "!" << std::endl;
    std::cout << "Commands you can use : INVITE KICK LIST MODE QUIT PRIVMSG/NOTICE PART TOPIC NICK" << std::endl;
    return user->getServer()->toSend(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
                                                   user->getNick()),
                                     user->getFd());
}

void join_channel(Channel *chan, User *user)
{
    std::string channel = chan->getName();
    if (chan->isBanned(user->getNick()) == 1)
        return user->getServer()->toSend(ERR_BANNEDFROMCHAN(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (chan->getUserNum() >= chan->getUserLimit())
        return user->getServer()->toSend(ERR_CHANNELISFULL(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (user->getChannelList().size() >= static_cast<unsigned int>(user->getChannelLimit()))
        return user->getServer()->toSend(ERR_TOOMANYCHANNELS(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (chan->isInvited(user->getNick()) == false && chan->isInviteOnly() == 1)
        return user->getServer()->toSend(ERR_INVITEONLYCHAN(getArgs(channel), user->getNick()),
                                         user->getFd());

    std::cout << "Joined the chan" << std::endl;
    user->getServer()->toSend(getMsg(user, "JOIN", channel),
                              chan->getFds());
    chan->addUser(user);
    user->addChannel(chan->getName());
    if (chan->getTopic() != "")
        user->getServer()->toSend(RPL_TOPIC(getArgs(channel, chan->getTopic()),
                                            user->getNick()),
                                  user->getFd());
    else
        user->getServer()->toSend(RPL_NOTOPIC(getArgs(channel),
                                              user->getNick()),
                                  user->getFd());
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
        // std::cout << ' ' << *it;
    }
    // a garder car dans la doc https://www.rfc-editor.org/rfc/rfc2812#section-3.2
    std::cout << "You have joined the channel " << reply_channel << "!" << std::endl;
    std::cout << "Commands you can use : INVITE KICK LIST MODE QUIT PRIVMSG/NOTICE PART TOPIC NICK" << std::endl;
    return user->getServer()->toSend(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
                                                   user->getNick()),
                                     user->getFd());
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
    std::map<std::string, Channel *> channelList = user->getServer()->getChannelList();
    if (user->param_list[0] == "0" || user->param_list[0] == "#0")
    {
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            std::cout << RED "Part " << it->first << E << std::endl;
            user->toCommand("PART " + it->first);
        }
        return;
    }

    std::string key;
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
        Channel *chan = user->getServer()->getChannel(listNewChans[i]);
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
                user->getServer()->toSend(ERR_BADCHANNELKEY(getArgs(listNewChans[i]), user->getNick()),
                                          user->getFd());
        }
        i++;
    }

    std::cout << YELLOW "all chan + pwd + nick in the chan" E << std::endl;
    std::map<std::string, Channel *> channelsList = user->getServer()->getChannelList();
    for (std::map<std::string, Channel *>::iterator it = channelsList.begin(); it != channelsList.end(); ++it)
    {
        std::cout << it->first << "   ";
        std::cout << it->second->getKey() << std::endl;
        std::vector<std::string> listNick = it->second->getNickList();
        for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
            std::cout << ' ' << *it << std::endl;
    }
}
