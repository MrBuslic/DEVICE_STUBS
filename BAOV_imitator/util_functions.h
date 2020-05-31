#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
class util_functions
{
public:
	util_functions();
	~util_functions();
	static int parseValue(std::istringstream& lineStream);
};

