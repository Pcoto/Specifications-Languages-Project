/*  rl_enc_ref.h */ 
#ifndef _RL_ENC_REF
#define _RL_ENC_REF 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc_RTL) {

	sc_in<sc_int<9> > 	input;
	sc_out<sc_int<9> >	output;
	sc_in<bool>		  	ready_i;
	sc_out<bool>		ask_i;
	sc_out<bool>		ready_o;
	sc_in<bool>			ask_o;
	sc_in_clk			clk;
	sc_in<bool>			reset;

	enum		ctrl_state {RESET, READACDC, WRITEACDC, WAITACDC, READZERO, CHECKZERO, WRITECOUNT, WAITCOUNT, TERMINATION};
	sc_signal<ctrl_state>	state;
	sc_signal< sc_int<9> >	value;
	sc_signal< sc_uint<6> >	count;
	sc_signal< sc_uint<7> > k;

	SC_HAS_PROCESS(rl_enc_RTL);

	rl_enc_RTL(sc_module_name name):
		sc_module(name)
	{
		SC_METHOD(process);
		sensitive << clk.pos();
	}

	void process();
};

#endif
