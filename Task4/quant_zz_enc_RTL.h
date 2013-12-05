/*  quant_zz_enc_RTL.h */
#ifndef _QUANT_ZZ_ENC_RTL
#define _QUANT_ZZ_ENC_RTL
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

// RTL model of rl_dec

SC_MODULE(quant_zz_enc_RTL) {

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
	enum		ctrl_state {RESET, READVALUE, WAITSAVE, ZZREAD, WRITEVALUES, WAITACDC, WRITECOUNT, WAITCOUNT,TERMINATION};
	sc_signal<ctrl_state>	state;//	ctrl_state	state;
	sc_signal< int >	count_j,count_i,count_k;
	sc_signal< int >	count_1,count_2;
  sc_signal<int>		temp_block[64];
  sc_signal<sc_int<9> > block[64]; 
	// sc_signal< int >	value;
	sc_signal< bool >	flag;

	// processes
	void process_RTL();

  int* quantization;

  SC_HAS_PROCESS(quant_zz_enc_RTL);

  quant_zz_enc_RTL(sc_module_name name, int* _quantization):
		sc_module(name),
		quantization(_quantization){
		SC_METHOD(process_RTL);
		sensitive << clk.pos() ;
	}

};

#endif
