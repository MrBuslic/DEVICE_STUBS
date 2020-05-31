#include "drs.h"
#include "util_functions.h"


drs::drs()
{
}


drs::~drs()
{
}

sensor drs::parseSensor(std::istringstream & lineStream, drs & object)
{
	sensor::parseSensor(lineStream, object);
	object.time = util_functions::parseValue(lineStream);
	object.amplitude = util_functions::parseValue(lineStream);
	object.background = util_functions::parseValue(lineStream);
	return object;
}

int drs::get_field_value(int field_number)
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
			return background;
		}
		return 0;
	}
}