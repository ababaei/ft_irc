#ifndef REGISTER_HPP
# define REGISTER_HPP

# include <iostream>
# include <string>
# include <string.h>

class Register
{

	public:

		Register();
		Register( Register const & src );
		~Register();

		Register &		operator=( Register const & rhs );

		std::string		handle_new_entry(std::string str);

		std::string		get_nick();
		std::string		get_pass();
		std::string		get_cap();

	private:

		std::string nick;
		std::string cap;
		std::string pass;

};

std::ostream &			operator<<( std::ostream & o, Register const & i );

#endif /* ******************************************************** REGISTER_H */