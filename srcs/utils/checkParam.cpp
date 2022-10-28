bool	isChanName(const std::string& param)
{
	if (param[0] == '&' || param[0] == '#' || param[0] == '+' || param[0] == '!')
		return true;
	return false
}
