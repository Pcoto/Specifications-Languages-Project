/*  DCT_compressor.h */ 
#ifndef _DCT_compressor
#define _DCT_compressor 
#include <systemc.h>
#include "add2systemc.h"

#include	"quantize.h"
#include	"zz_enc.h"
#include	"rl_enc.h"
#include "FF2P.h"

SC_MODULE(dct_compressor) {
  

  my_fifo_in<float>   input;
  sc_out<sc_int<9> >   output;
  sc_in<bool>  ask;
  sc_in<bool>  clk;
  sc_out<bool>  ready;
  
  // int* quantization;
  
  fifo_stat<int>	quant_out;
  fifo_stat<int>	zz_enc_out;
  fifo_stat<int>	rl_enc_out;
  
  quantize quant_1;
  zz_enc zz_enc_1;
  rl_enc rl_enc_1;
  
  FF2PC <int, sc_int<9> > FF2PC_1;

  // SC_HAS_PROCESS(dct_compressor);

  dct_compressor(sc_module_name name, int* _quantization):
		sc_module(name),
    quant_out("quant_out",1),
		zz_enc_out("zz_enc_out",1),
    rl_enc_out("rl_enc_out",1),
		quant_1("quant_1",_quantization),
		zz_enc_1("zz_enc_1"),
		rl_enc_1("rl_enc_1"),
    FF2PC_1("FF2PC_1") {
    
      quant_1.input(input);
      quant_1.output(quant_out);
    
      zz_enc_1.input(quant_out);
      zz_enc_1.output(zz_enc_out);
    
      rl_enc_1.input(zz_enc_out);
      rl_enc_1.output(rl_enc_out);
      
      FF2PC_1.input(rl_enc_out);
      FF2PC_1.ask(ask);
      FF2PC_1.ready(ready);
      FF2PC_1.clk(clk);
      FF2PC_1.output(output);
			// SC_THREAD(process);
		}
  
  void process();
};

#endif
