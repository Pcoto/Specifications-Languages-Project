#include "pack_data_RTL.h"

void pack_data_RTL::pack()
{
	sc_int<VALUE_BITS>	value2;
	bool				rem;
	sc_uint<RL_BITS>	zeros;
	sc_uint<8>			tmp_byte;

	if (reset.read() == false)
	{
		ask_i.write(false);
		ready_o.write(false);
		state.write(RESET);
	}
	else
	{
		switch (state.read())
		{
			case RESET:
				ask_i.write(true);
				ready_o.write(false);
				state.write(READVAL);
				break;

			case READVAL:
				ready_o.write(false);
				ask_i.write(true);
				if(ready_i.read() == true)
				{
					in_data.write(input.read());
					ask_i.write(false);
					state.write(WRITEVAL);
				}
				break;

			case WRITEVAL:
				if(ask_o.read() == true)
				{
					value2 = in_data.read();
					output.write(static_cast < sc_uint<8> >(value2.range(8,1)));
					ready_o.write(true);
					state.write(WAITVAL);
				}
				else
				{
					ready_o.write(false);
					ask_i.write(false);
					state.write(WRITEVAL);
				}
				break;

			case WAITVAL:
				ready_o.write(false);
				value2 = in_data.read();
				rem = value2[0];
				value2 = 0;
				value2[7] = rem;
				out_byte.write(static_cast < sc_uint<8> >(value2));
				ask_i.write(true);
				state.write(READZERO);
				break;

			case READZERO:
				ready_o.write(false);
				if(ready_i.read() == true)
				{
					in_data.write(input.read());
					ask_i.write(false);
					state.write(WRITEZERO);
				}
				break;

			case WRITEZERO:
				if(ask_o.read() == true)
				{
					value2 = in_data.read();
					zeros = value2.range(6,0);
					tmp_byte = out_byte.read();
					tmp_byte.range(6,0) = zeros;
					output.write(tmp_byte);
					ready_o.write(true);
					ask_i.write(false);
					state.write(READVAL);
				}
				break;
		}
	}



//	sc_int<VALUE_BITS>	in_data;
//	sc_uint<8>			out_byte;
//	sc_int<VALUE_BITS>	value2;
//
//	while (1)
//	{
//		ask_i.write(true);
//		wait();
//		while (ready_i.read()!= true) wait();
//		in_data = input.read();
//		ask_i.write(false);
//		wait();
//
//		while (ask_o.read()!= true) wait();
//		value2 = in_data;
//		output.write(static_cast < sc_uint<8> >(value2.range(8,1)));
//		ready_o.write(true);
//		wait();
//		ready_o.write(false);
//		wait();
//		out_byte = 0;
//		out_byte[7] = in_data[0];
//		ask_i.write(true);
//		wait();
//		while (ready_i.read()!= true) wait();
//		in_data = input.read();
//		ask_i.write(false);
//		wait();
//
//		while (ask_o.read()!= true) wait();
//		out_byte.range(6,0) = in_data.range(6,0);
//		output.write(out_byte);
//		ready_o.write(true);
//		wait();
//		ready_o.write(false);
//		wait();
//	}
}
