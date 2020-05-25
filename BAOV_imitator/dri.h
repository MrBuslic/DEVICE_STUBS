#pragma once
#include "sensor.h"
#include "dri_element.h"
class dri :
	public sensor
{
public:
	dri();
	~dri();
	int get_field_value(int field_number);
	sensor parseSensor(std::istringstream& lineStream, dri & object);
	int container_temperature;
	dri_element *dri_elements[16];
};


