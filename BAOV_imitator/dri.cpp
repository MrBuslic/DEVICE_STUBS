#include "dri.h"
#include "util_functions.h"


dri::dri()
{
}


dri::~dri()
{
}

sensor dri::parseSensor(std::istringstream & lineStream, dri & object)
{
	sensor::parseSensor(lineStream, object);
	for (int i = 0; i < 16; i++) {
		object.dri_elements[i] = new dri_element();
		object.dri_elements[i]->amplitude = util_functions::parseValue(lineStream);
		object.dri_elements[i]->background = util_functions::parseValue(lineStream);
	}
	return object;
}

int dri::get_field_value(int field_number)
{
	int value = sensor::get_field_value(field_number);
	if (value != 0)
		return value;
	else {
		switch (field_number) {
		case 3:
			return container_temperature;
		case 4:
			return dri_elements[0]->amplitude;
		case 5:
			return dri_elements[0]->background;
		case 6:
			return dri_elements[1]->amplitude;
		case 7:
			return dri_elements[1]->background;
		case 8:
			return dri_elements[2]->amplitude;
		case 9:
			return dri_elements[2]->background;
		case 10:
			return dri_elements[3]->amplitude;
		case 11:
			return dri_elements[3]->background;
		case 12:
			return dri_elements[4]->amplitude;
		case 13:
			return dri_elements[4]->background;
		case 14:
			return dri_elements[5]->amplitude;
		case 15:
			return dri_elements[5]->background;
		case 16:
			return dri_elements[6]->amplitude;
		case 17:
			return dri_elements[6]->background;
		case 18:
			return dri_elements[7]->amplitude;
		case 19:
			return dri_elements[7]->background;
		case 20:
			return dri_elements[8]->amplitude;
		case 21:
			return dri_elements[8]->background;
		case 22:
			return dri_elements[9]->amplitude;
		case 23:
			return dri_elements[9]->background;
		case 24:
			return dri_elements[10]->amplitude;
		case 25:
			return dri_elements[10]->background;
		case 26:
			return dri_elements[11]->amplitude;
		case 27:
			return dri_elements[11]->background;
		case 28:
			return dri_elements[12]->amplitude;
		case 29:
			return dri_elements[12]->background;
		case 30:
			return dri_elements[13]->amplitude;
		case 31:
			return dri_elements[13]->background;
		case 32:
			return dri_elements[14]->amplitude;
		case 33:
			return dri_elements[14]->background;
		case 34:
			return dri_elements[15]->amplitude;
		case 35:
			return dri_elements[15]->background;
		}
		return 0;
	}
}
