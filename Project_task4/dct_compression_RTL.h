/*  dct_compression_RTL.h */
#ifndef _DCT_COMPRESSION_RTL
#define _DCT_COMPRESSION_RTL

#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(dct_compression_RTL)
{
	// ports
	sc_in< sc_fixed<22,12,SC_TRN,SC_WRAP> >	input;
	sc_out< sc_uint<8> >  					output;
	sc_in<bool>		  						ready_i;
	sc_in<bool>		  						reset;
	sc_out<bool>							ask_i;
	sc_out<bool>							ready_o;
	sc_in<bool>								ask_o;
	sc_in_clk								clk;

	// variables
	enum					ctrl_state {RESET, READVALUE, WAITSAVE, ZZREAD, WRITEVALUES, WAITACDC, WRITECOUNT, WAITCOUNT, TERMINATION};
	sc_signal< sc_uint<4> >	state;
	sc_signal< sc_int<7> >	count_i;
	sc_signal< sc_uint<3> >	count_j;
	sc_signal< sc_uint<7> >	count_k;
	sc_signal< sc_int<9> >	temp_block[64];
	sc_signal< sc_int<9> > 	block[64];
	sc_signal< sc_int<9> > 	firstByte;
	sc_signal<bool>			flag;

	int* 					quantization;

	// processes
	void process_RTL();

	SC_HAS_PROCESS(dct_compression_RTL);

	dct_compression_RTL(sc_module_name name, int* _quantization):
		sc_module(name),
		quantization(_quantization)
	{
		SC_METHOD(process_RTL);
		sensitive << clk.pos();
	}
};

#endif
