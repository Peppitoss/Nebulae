#include"GenericString.h"
#include <algorithm>

std::string lower(std::string& data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

bool strequal(std::string& str1, std::string str2)
{
	return lower(str1) == lower(str2);
}
