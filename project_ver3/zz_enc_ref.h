/*  zz_enc_ref.h */ 
#ifndef _ZZ_ENC_REF
#define _ZZ_ENC_REF  
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(zz_enc_ref) {
  
  my_fifo_in<sc_int<9> >   input;
  my_fifo_out<sc_int<9> >  output;

  SC_HAS_PROCESS(zz_enc_ref);

  zz_enc_ref(sc_module_name name): 
	sc_module(name) {
		SC_THREAD(process);
	}
  
  void process();
};

#endif
