#include "brgi.h"
#include "util_functions.h"


brgi::brgi()
{
}


brgi::~brgi()
{
}

int brgi::get_field_value(int field_number)
{
	int value = sensor::get_field_value(field_number);
	if (value != 0)
		return value;
	else {
		switch (field_number) {
		case 3: 
			return time;
		case 4:
			return amplitude;
		case 5:
			return temperature;
		case 6:
			return interference;
		}
	}
	return 0;
}

sensor brgi::parseSensor(std::istringstream & lineStream, brgi & object)
{
	sensor::parseSensor(lineStream, object);
	object.time = util_functions::parseValue(lineStream);
	object.amplitude = util_functions::parseValue(lineStream);
	object.temperature = util_functions::parseValue(lineStream);
	object.interference = util_functions::parseValue(lineStream);
	return object;
}

