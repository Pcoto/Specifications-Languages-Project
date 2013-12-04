/*  jpeg_enc.h */ 
#include	"r2b.h"
#include	"dct.h"
// #include	"quantize.h"
// #include	"zz_enc.h"
// #include	"rl_enc.h"
// #include "FF2P.h"

#include "dct_compressor.h"
 

SC_MODULE(jpeg_enc) {
  
  /* in and output */
  my_fifo_in<int>   input;
  my_fifo_in<int>   parameters;
  sc_out<sc_int<9> >  output;
  sc_in<bool>  ask;
  sc_in<bool>  clk;
  sc_out<bool>  ready;

  /* internal FIFOs */
  fifo_stat<int> 	r2b_out;
  fifo_stat<float> 	dct_out;
  fifo_stat<int>	quant_out;
  fifo_stat<int>	zz_enc_out;
  fifo_stat<int>	rl_enc_out;

  /* internal modules */
  r2b r2b_1;
  dct dct_1;
  // quantize quant_1;
  // zz_enc zz_enc_1;
  // rl_enc rl_enc_1;
  dct_compressor dct_compressor_1;
  
  // FF2PC <int, sc_int<9> > FF2PC_1;

  jpeg_enc(sc_module_name _name, int* _quantization, int _maxwidth):
		sc_module(_name),
		r2b_out("r2b_out",1),
		dct_out("dct_out",1),
		// quant_out("quant_out",1),
		// zz_enc_out("zz_enc_out",1),
    // rl_enc_out("rl_enc_out",1),
		r2b_1("r2b_1",_maxwidth),
		dct_1("dct_1"),
		dct_compressor_1("dct_compressor_1",_quantization)
		{

	r2b_1.input(input);
	r2b_1.parameters(parameters);
	r2b_1.output(r2b_out);

	dct_1.input(r2b_out);
	dct_1.output(dct_out);
  
  dct_compressor_1.input(dct_out);
  dct_compressor_1.ask(ask);
  dct_compressor_1.clk(clk);
  dct_compressor_1.ready(ready);
  dct_compressor_1.output(output);
  
  

	// quant_1.input(dct_out);
	// quant_1.output(quant_out);

	// zz_enc_1.input(quant_out);
	// zz_enc_1.output(zz_enc_out);

	// rl_enc_1.input(zz_enc_out);
	// rl_enc_1.output(rl_enc_out);
  
  // FF2PC_1.input(rl_enc_out);
  // FF2PC_1.ask(ask);
  // FF2PC_1.ready(ready);
  // FF2PC_1.clk(clk);
  // FF2PC_1.output(output);
  

		}
  
  ~jpeg_enc() {
	}

};
