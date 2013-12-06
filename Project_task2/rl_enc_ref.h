/*  rl_enc_ref.h */ 
#ifndef _RL_ENC_REF
#define _RL_ENC_REF 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc_ref) {
  
  my_fifo_in<sc_int<9> >   input;
  my_fifo_out<sc_int<9> >  output;

  SC_HAS_PROCESS(rl_enc_ref);

  rl_enc_ref(sc_module_name name):
		sc_module(name) {
			SC_THREAD(process);
		}
  
  void process();
};

#endif
