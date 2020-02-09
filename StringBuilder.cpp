#include"StringBuilder.h"
#include <sstream>
#include<iomanip>

std::string IntToString(int input)
{
	std::ostringstream Convert;
	Convert << input;
	return Convert.str();
}

std::string DoubleToString(double input, int precision)
{
	std::ostringstream Convert;
	Convert << std::setprecision(precision) << input;
	return Convert.str();
}