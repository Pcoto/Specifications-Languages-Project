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
      ask_i.write(true);
      wait();
      while (ready_i.read()!= true) wait();
			value = input.read();
      ask_i.write(false);
      wait();
			out_byte = value.range(8,1);
      
      while (ask_o.read()!= true) wait();
			output.write(out_byte);
      ready_o.write(true);
      wait();
      ready_o.write(false);
      wait();
			out_byte = 0;
			out_byte[7] = value[0];
      ask_i.write(true);
      wait();
      while (ready_i.read()!= true) wait();
			value = input.read();
      ask_i.write(false);
      wait();
			out_byte.range(6,0) = value.range(6,0);
      
      while (ask_o.read()!= true) wait();
			output.write(out_byte);
      ready_o.write(true);
      wait();
      ready_o.write(false);
      wait();
		}
	}
	return;
}
