/*  rl_enc.h */ 
#ifndef _RL_ENC
#define _RL_ENC 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc_pack) {
  
  my_fifo_in<int>   input;
  my_fifo_out<sc_int<9> >  output;

  SC_HAS_PROCESS(rl_enc_pack);

  rl_enc_pack(sc_module_name name):
		sc_module(name) {
			SC_THREAD(process);
		}
  
  void process();
};

#endif
