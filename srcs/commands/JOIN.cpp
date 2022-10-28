#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void JOIN(User *User)
{
    std::cout << "Hello JOIN" << std::endl;
    // check debut is # ou & quand cest pas une key
    // check needmore param s'il faut une key (ERR_NEEDMOREPARAMS)
    // Key correct (ERR_BADCHANNELKEY)
    // check que cest pas sur invite (ERR_INVITEONLYCHAN)
    // Channel names may contain a maximum of 164 characters. This limit includes channel prefixes (i.e. private- and presence- are included in the character count). ERR_NOSUCHCHANNEL
    // Channel names should only include lower and uppercase letters, numbers and the following punctuation _ - = @ , . ; ERR_NOSUCHCHANNEL
    // Cannot join chan is banned (ERR_BANNEDFROMCHAN)
    // ERR_CHANNELISFULL (500)
    //joined too many chans : ERR_TOOMANYCHANNELS
    // ERR_TOOMANYTARGETS : Returned to a client which is attempting to JOIN a safe
    //        channel using the shortname when there are more than one
    //        such channel. --> Necessaire ?
    //ERR_BADCHANMASK --> Necessaire ?
    //ERR_UNAVAILRESOURCE ? 
    // JOIN 0 = leave all channel grace via PART
    int j = 0;
    while (j <= User->param_list.size())
    {
        std::cout << "params JOINS are: " << User->param_list[j] << std::endl;
        j++;
    }

    if (User->param_list[0] == "0")
    {
        // Part(all_channels);
    }
    else
    {
        int i = 0;
        while (i < User->param_list.size())
        {
            User->param_list[i].erase(0, 1);
            std::vector<std::string> channel_list = User->get_server()->get_channels_list();
            if (std::find(channel_list.begin(), channel_list.end(), User->param_list[i]) == channel_list.end())
            {
                User->get_server()->add_channel(User->param_list[i]);
                // add operator mode for this chan for this user
            }
            else
            {
                // joindre le chan deja cree
                // RPL_TOPIC
                // RPL_NAMREPLY (353)
                // RPL_ENDOFNAMES (366).
            }
            i++;
        }
    }
    std::vector<std::string> channel_list = User->get_server()->get_channels_list();

    std::cout << "Now list chan is" << std::endl;
    for (std::vector<std::string>::iterator it = channel_list.begin(); it != channel_list.end(); ++it)
        std::cout << " " << *it;
}