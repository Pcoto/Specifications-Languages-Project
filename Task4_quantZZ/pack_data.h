#ifndef PACK_DATA_H_
#define PACK_DATA_H_

#include <systemc.h>
#include "add2systemc.h"

#define VALUE_BITS	9
#define RL_BITS		6

SC_MODULE(pack_data)
{
	sc_in< sc_int<VALUE_BITS> > input;
	sc_out< sc_uint<8> >	output;

	sc_in<bool>		  ready_i;
	sc_out<bool>		  ask_i;
	sc_out<bool>		  ready_o;
	sc_in<bool>		  ask_o;
	sc_in_clk			   clk;

	SC_HAS_PROCESS(pack_data);

	pack_data(sc_module_name name):
		sc_module(name)
	{
		SC_THREAD(pack);
		sensitive<<clk.pos();
	}

	void pack();
private:
	sc_int<VALUE_BITS>	val, rl;
	sc_uint<8>			out_byte;
	int					offset;
	void split_and_pack(sc_int<VALUE_BITS> input_data, int num_bits);
};

#endif /* PACK_DATA_H_ */
