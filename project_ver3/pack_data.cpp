#include "pack_data.h"

void pack_data::pack()
{
	sc_int<VALUE_BITS>	value = 0;
	sc_uint<RL_BITS>	rl = 0;
	sc_uint<8>			out_byte;
	int 				max = 0;
	while (1)
	{
		value = 0;
		while(value != 63)
		{
			value = input.read();
			out_byte = value.range(8,1);
			output.write(out_byte);
			out_byte = 0;
			out_byte[7] = value[0];
			value = input.read();
			out_byte.range(6,0) = value.range(6,0);
			output.write(out_byte);
		}
	}
	return;
}
