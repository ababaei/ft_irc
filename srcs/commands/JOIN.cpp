#include "../../inc/command.hpp"
#include "../../inc/Server.hpp"

void JOIN(User *User)
{
    std::cout << "Hello JOIN" << std::endl;
    //Faire boucle car on peut join plusieurs chan avec la meme cmd JOIN 
    //JOIN 0 = leave all channel grace via PART
    User->param_list[0].erase(0,1);
    int i = 0;
    while (i < User->param_list.size())
    {
        std::cout << "params JOINS are: " << User->param_list[i] << std::endl;
        i++;
    }
    std::vector<std::string> channel_list = User->get_server()->get_channels_list();
    if (std::find(channel_list.begin(), channel_list.end(), User->param_list[0]) == channel_list.end())
        User->get_server()->add_channel(User->param_list[0]);
    else
    {
        //joindre le chan deja cree
    }
    
    std::vector<std::string> lala = User->get_server()->get_channels_list();

    std::cout << "Now list chan is" << std::endl;
	for (std::vector<std::string>::iterator it = lala.begin(); it != lala.end(); ++it)
		std::cout << " " << *it;
}