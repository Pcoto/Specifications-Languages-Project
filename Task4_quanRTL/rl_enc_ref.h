/*  rl_enc_ref.h */ 
#ifndef _RL_ENC_REF
#define _RL_ENC_REF 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc_ref) {
  
  sc_in<sc_int<9> >   input;
  sc_out<sc_int<9> >  output;
  sc_in<bool>		  ready_i;
  sc_out<bool>		  ask_i;
  sc_out<bool>		  ready_o;
  sc_in<bool>		  ask_o;
  sc_in_clk			   clk;

  SC_HAS_PROCESS(rl_enc_ref);

  rl_enc_ref(sc_module_name name):
		sc_module(name) {
			SC_THREAD(process);
			sensitive<<clk.pos();
		}
  
  void process();
};

#endif
