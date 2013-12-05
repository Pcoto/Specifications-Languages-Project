#ifndef PACK_DATA_RTL_H_
#define PACK_DATA_RTL_H_

#include <systemc.h>
#include "add2systemc.h"

#define VALUE_BITS	9
#define RL_BITS		7

SC_MODULE(pack_data_RTL)
{
	sc_in< sc_int<VALUE_BITS> > input;
	sc_out< sc_uint<8> >		output;

	sc_in<bool>		ready_i;
	sc_out<bool>	ask_i;
	sc_out<bool>	ready_o;
	sc_in<bool>		ask_o;
	sc_in_clk		clk;
	sc_in<bool>		reset;

	enum		ctrl_state {RESET, READVAL, WRITEVAL, WAITVAL, READZERO, WRITEZERO};
	sc_signal<ctrl_state>			state;
	sc_signal< sc_int<VALUE_BITS> >	in_data;
	sc_signal< sc_uint<8> >			out_byte;

	SC_HAS_PROCESS(pack_data_RTL);

	pack_data_RTL(sc_module_name name):
		sc_module(name)
	{
		SC_METHOD(pack);
		sensitive << clk.pos();
	}

	void pack();
};

#endif /* PACK_DATA_RTL_H_ */
