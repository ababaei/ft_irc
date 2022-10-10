#include "../inc/Register.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Register::Register()
{
}

Register::Register( const Register & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Register::~Register()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Register &				Register::operator=( Register const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Register const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string		Register::handle_new_entry(std::string str)
{
	std::string key = str.substr(0, str.find(" "));
	//std::cout << str << std::endl;
	if (!key.compare("CAP"))
		this->cap = str.substr(str.find(" ") + 1, str.find("\n") - str.find(" ") - 2);
	else if (!key.compare("PASS"))
		this->pass = str.substr(str.find(" ") + 1, str.find("\n") - str.find(" ") - 2);
	else if (!key.compare("NICK"))
		this->nick = str.substr(str.find(" ") + 1, str.find("\n") - str.find(" ") - 2);
	str = str.substr(str.find("\n") + 1, str.length() - str.find("\n") - 1);
	return (key);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Register::get_cap() {return (this->cap); }
std::string		Register::get_nick() {return (this->nick); }
std::string		Register::get_pass() {return (this->pass); }

/* ************************************************************************** */