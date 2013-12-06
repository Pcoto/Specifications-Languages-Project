/*  dct_ref.h */ 
#ifndef _DCT_REF
#define _DCT_REF
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(dct_ref) {
  
  my_fifo_in<int>   input;
  my_fifo_out <sc_fixed<22,12,SC_TRN,SC_WRAP> >  output;

  SC_CTOR(dct_ref) { SC_THREAD(process);}
  
  void process();
};

#endif
