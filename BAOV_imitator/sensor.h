#pragma once
#include <string>
#include <map>
using namespace std;
class sensor
{
public:
	sensor();
	~sensor();
	string name;
	virtual int get_field_value(int field_number);
	sensor parseSensor(std::istringstream& lineStream, sensor & object);
	int address;
	int active;
	int enabled;
	int detection_threshold;
};

