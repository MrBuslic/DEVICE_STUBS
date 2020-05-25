#pragma once
#include "sensor.h"
class drs :
	public sensor
{
public:
	drs();
	~drs();
	int get_field_value(int field_number);
	sensor parseSensor(std::istringstream& lineStream, drs & object);
	int time;
	int amplitude;
	int background;
};

