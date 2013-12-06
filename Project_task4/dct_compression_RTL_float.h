/*  dct_compression_RTL.h */
#ifndef _DCT_COMPRESSION_RTL
#define _DCT_COMPRESSION_RTL

#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(dct_compression_RTL_float)
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
	enum					ctrl_state {RESET, READVALUE, MULTIPLIER, WAITSAVE, ZZREAD, WRITEVALUES, WAITACDC, WRITECOUNT, WAITCOUNT, TERMINATION};
	sc_signal< sc_uint<4> >	state;
	sc_signal< sc_int<7> >	count_i;
	sc_signal< sc_uint<3> >	count_j;
	sc_signal< sc_uint<7> >	count_k;
	sc_signal< sc_int<9> >	temp_block[64];
	sc_signal< sc_int<9> > 	block[64];
	sc_signal< sc_int<9> > 	firstByte;
	sc_signal<bool>			flag;
  
  sc_signal<sc_ufixed<14,-2,SC_TRN,SC_WRAP> >	quantization_temp;

	sc_ufixed<14,-2,SC_TRN,SC_WRAP>*	quantization_i;

	// processes
	void process_RTL();

	SC_HAS_PROCESS(dct_compression_RTL_float);

	dct_compression_RTL_float(sc_module_name name, sc_ufixed<14,-2,SC_TRN,SC_WRAP>* _quantization_i):
		sc_module(name),
		quantization_i(_quantization_i)
	{
		SC_METHOD(process_RTL);
		sensitive << clk.pos();
	}
};

#endif
