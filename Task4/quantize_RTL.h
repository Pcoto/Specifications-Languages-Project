/*  quantize_RTL.h */
#ifndef _QUANTIZE_RTL
#define _QUANTIZE_RTL
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

// RTL model of rl_dec

SC_MODULE(quantize_RTL) {

	// ports
  sc_in<sc_fixed<22,12,SC_TRN,SC_WRAP> >  input;
  sc_out<sc_int<9> >  output;
  sc_in<bool>		  ready_i;
  sc_in<bool>		  reset;
  sc_out<bool>		  ask_i;
  sc_out<bool>		  ready_o;
  sc_in<bool>		  ask_o;
  sc_in_clk			   clk;

	// variables
	enum		ctrl_state {RESET, READVALUE, WRITEVALUE, WAITWRITE, TERMINATION};
	sc_signal<ctrl_state>	state;//	ctrl_state	state;
	sc_signal< int >	count_1,count_2;
	sc_signal< int >	value;

	// processes
	void process_RTL();

  int* quantization;

  SC_HAS_PROCESS(quantize_RTL);

  quantize_RTL(sc_module_name name, int* _quantization):
		sc_module(name),
		quantization(_quantization){
		SC_METHOD(process_RTL);
		sensitive << clk.pos() ;
	}

};

#endif
