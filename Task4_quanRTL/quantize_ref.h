/*  quantize_ref.h */ 
#ifndef _QUANTIZE_REF
#define _QUANTIZE_REF 
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(quantize_ref) {
  
  sc_in<sc_fixed<22,12,SC_TRN,SC_WRAP> >  input;
  sc_out<sc_int<9> >  output;
  sc_in<bool>		  ready_i;
  sc_out<bool>		  ask_i;
  sc_out<bool>		  ready_o;
  sc_in<bool>		  ask_o;
  sc_in_clk			   clk;

  int* quantization;

  SC_HAS_PROCESS(quantize_ref);

  quantize_ref(sc_module_name name, int* _quantization):
		sc_module(name),
		quantization(_quantization) {
			SC_THREAD(process);
			sensitive << clk.pos();
		}
  
  void process();
};

#endif
