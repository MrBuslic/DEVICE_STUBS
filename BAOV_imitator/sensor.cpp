#include "sensor.h"
#include "util_functions.h"



sensor::sensor()
{
}


sensor::~sensor()
{
}

int sensor::get_field_value(int field_number)
{
	switch (field_number) {
	case 0:
		return address;
	case 1:
		return active;
	case 2:
		return enabled;
	}
	return 0;
}

sensor sensor::parseSensor(std::istringstream & lineStream, sensor & object)
{
	object.address = util_functions::parseValue(lineStream);
	object.active = util_functions::parseValue(lineStream);
	object.enabled = util_functions::parseValue(lineStream);
	object.detection_threshold = 5;
	return object;
}


