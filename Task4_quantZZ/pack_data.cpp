#include "pack_data.h"

void pack_data::pack()
{
	while (1)
	{
		offset = 0;
		rl = 0;
		while(rl != 63)
		{
		  ask_i.write(true);
		  wait();
		  while (ready_i.read()!= true) wait();
		  val = input.read();
		  ask_i.write(false);
		  wait();
		  split_and_pack(val, VALUE_BITS);
		  ask_i.write(true);
		  wait();
		  while (ready_i.read()!= true) wait();
		  rl = input.read();
		  ask_i.write(false);
		  wait();
		  split_and_pack(rl, RL_BITS);
		}
		if(offset != 0)
		{
			out_byte.range(7-offset, 0) = 0;
			while (ask_o.read()!= true) wait();
			output.write(out_byte);
			ready_o.write(true);
			wait();
			ready_o.write(false);
			wait();
		}
	}
}

void pack_data::split_and_pack(sc_int<VALUE_BITS> input_data, int num_bits)
{
	int range_start, range_end;

	int mul = (offset+num_bits) / 8;
	int rem = (offset+num_bits) % 8;

	if (mul > 0)
	{
		range_start = num_bits-1;
		range_end	= num_bits   - (8-offset);
		out_byte.range(7-offset, 0) = input_data.range(range_start, range_end);
		while (ask_o.read()!= true) wait();
		output.write(out_byte);
		ready_o.write(true);
	    wait();
		ready_o.write(false);
		wait();

		for (int i = 1; i < mul; i++)
		{
			range_start = num_bits-1 - (8*i	   -offset);
			range_end   = num_bits   - (8*(i+1)-offset);
			out_byte = input_data.range(range_start, range_end);
			while (ask_o.read()!= true) wait();
			output.write(out_byte);
			ready_o.write(true);
			wait();
			ready_o.write(false);
			wait();
		}

		if(rem != 0)
		{
			out_byte.range(7, 8-rem) = input_data.range(rem-1, 0);
		}
	}
	else
	{
		out_byte.range(7-offset, 8-rem) = input_data;
	}

	offset = rem;
}

