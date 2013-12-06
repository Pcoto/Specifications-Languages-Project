/*  jpeg_enc.h */ 
#include	"r2b.h"
#include	"dct.h"
#include 	"dct_compression.h"


SC_MODULE(jpeg_enc) {

	/* in and output */
	my_fifo_in<int>   input;
	my_fifo_in<int>   parameters;
	sc_out<sc_uint<8> >  output;
	sc_in<bool>  ask;
	sc_in<bool>  clk;
	sc_out<bool>  ready;

	/* internal FIFOs */
	fifo_stat<int> 	r2b_out;
	fifo_stat<float> 	dct_out;
	fifo_stat<sc_uint<8> >	pack_out;

	/* internal modules */
	r2b r2b_1;
	dct dct_1;
	dct_compressor dct_compressor_1;

	jpeg_enc(sc_module_name _name, int* _quantization, int _maxwidth):
		sc_module(_name),
		r2b_out("r2b_out",1),
		dct_out("dct_out",1),
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

	}

	~jpeg_enc() {
	}

};
