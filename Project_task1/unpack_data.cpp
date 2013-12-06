#include "unpack_data.h"

void unpack_data::unpack()
{
	sc_uint<8> input_data;
	sc_int<9> value;
	sc_uint<7> rl;

	while(1)
	{
		input_data = input.read();
		value = 0;
		value.range(8,1) = input_data;
		input_data = input.read();
		value[0] = input_data[7];
		output.write(value);
		rl = input_data.range(6,0);
		output.write(rl);
	}
}
