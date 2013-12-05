/*  zz_enc_ref.h */ 
#ifndef _ZZ_ENC_REF
#define _ZZ_ENC_REF  
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(zz_enc_ref) {
  
  sc_in<sc_int<9> >   input;
  sc_out<sc_int<9> >  output;
  sc_in<bool>		  ready_i;
  sc_out<bool>		  ask_i;
  sc_out<bool>		  ready_o;
  sc_in<bool>		  ask_o;
  sc_in_clk			   clk;
  
  SC_HAS_PROCESS(zz_enc_ref);

  zz_enc_ref(sc_module_name name): 
	sc_module(name) {
		SC_THREAD(process);
    sensitive << clk.pos();
	}
  
  void process();
};

#endif
