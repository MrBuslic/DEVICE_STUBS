#pragma once
#include "sensor.h"
class dni :
	public sensor
{
public:
	dni();
	~dni();
	int get_field_value(int field_number);
	sensor parseSensor(std::istringstream& lineStream, dni & object);
	int time;
	int amplitude;
	int container_temperature;
	int background;
};

