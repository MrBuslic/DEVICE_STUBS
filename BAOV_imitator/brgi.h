#pragma once
#include "sensor.h"
class brgi :
	public sensor
{
public:
	brgi();
	~brgi();
	int get_field_value(int field_number);
	sensor parseSensor(std::istringstream& lineStream, brgi & object);
	int time;
	int amplitude;
	int temperature;
	int interference;
};

