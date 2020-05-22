#pragma once
class functional_control_mapping_item
{
public:
	functional_control_mapping_item();
	~functional_control_mapping_item();
	int address;
	int is_enabled_sensor;
	int field_number;
	int message_number;
	int data_word_number;
	int start_bit;
	int bit_length;
};

