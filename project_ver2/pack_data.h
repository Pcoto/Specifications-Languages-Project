#ifndef PACK_DATA_H_
#define PACK_DATA_H_

#include <systemc.h>
#include "add2systemc.h"

#define VALUE_BITS	9
#define RL_BITS		7

SC_MODULE(pack_data)
{
	my_fifo_in< sc_int<VALUE_BITS> > input;
	my_fifo_out< sc_uint<8> >	output;

	SC_HAS_PROCESS(pack_data);

	pack_data(sc_module_name name):
		sc_module(name)
	{
		SC_THREAD(pack);
	}

	void pack();
};

#endif /* PACK_DATA_H_ */
