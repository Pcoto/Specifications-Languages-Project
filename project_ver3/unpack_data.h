#ifndef UNPACK_DATA_H_
#define UNPACK_DATA_H_

#include <systemc.h>
#include "add2systemc.h"

#define VALUE_BITS	9
#define RL_BITS		7

SC_MODULE(unpack_data)
{
	my_fifo_in<sc_uint<8> >	input;
	my_fifo_out<int> 	output;

	SC_HAS_PROCESS(unpack_data);

	unpack_data(sc_module_name name):
		sc_module(name)
	{
		SC_THREAD(unpack);
	}

	void unpack();
};

#endif /* UNPACK_DATA_H_ */
