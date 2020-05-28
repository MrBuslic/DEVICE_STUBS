#include "util_functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

util_functions::util_functions()
{
}


util_functions::~util_functions()
{
}

int util_functions::parseValue(std::istringstream & lineStream)
{
	std::string value;
	std::getline(lineStream, value, ' ');
	return std::stoi(value, 0);
}
