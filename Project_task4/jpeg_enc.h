/*  jpeg_enc.h */ 
#include	"genreset.h"
#include	"r2b.h"
#include	"dct.h"
#include	"FF2P.h"
#include 	"dct_compression_RTL.h"

SC_MODULE(jpeg_enc)
{
	/* in and output */
	my_fifo_in<int>   	input;
	my_fifo_in<int>   	parameters;
	sc_out<sc_uint<8> >	output;
	sc_in<bool>  		ask;
	sc_in<bool>  		clk;
	sc_out<bool>  		ready;

	/* internal FIFOs */
	fifo_stat<int> 			r2b_out;
	fifo_stat<float> 		dct_out;
	fifo_stat< sc_uint<8> >	pack_out;

	/* internal signals */
	sc_signal< sc_fixed<22,12,SC_TRN,SC_WRAP> >	FF2PC_out;
	sc_signal<bool> 							ask_in;
	sc_signal<bool> 							ready_in;
	sc_signal<bool> 							reset;

	/* internal modules */
	genreset										gen_reset_1;
	r2b 											r2b_1;
	dct 											dct_1;
	FF2PC<float, sc_fixed<22,12,SC_TRN,SC_WRAP> > 	FF2PC_1;
	dct_compression_RTL								dct_compression_1;

	jpeg_enc(sc_module_name _name, int* _quantization, int _maxwidth):
		sc_module(_name),
		r2b_out("r2b_out",1),
		dct_out("dct_out",1),
		gen_reset_1("gen_reset_1",1),
		r2b_1("r2b_1",_maxwidth),
		dct_1("dct_1"),
		FF2PC_1("FF2PC_1"),
		dct_compression_1("dct_compression_1",_quantization)
	{
		gen_reset_1.clk(clk);
		gen_reset_1.reset(reset);

		r2b_1.input(input);
		r2b_1.parameters(parameters);
		r2b_1.output(r2b_out);

		dct_1.input(r2b_out);
		dct_1.output(dct_out);

		FF2PC_1.input(dct_out);
		FF2PC_1.ask(ask_in);
		FF2PC_1.ready(ready_in);
		FF2PC_1.clk(clk);
		FF2PC_1.output(FF2PC_out);

		dct_compression_1.input(FF2PC_out);
		dct_compression_1.ask_i(ask_in);
		dct_compression_1.ask_o(ask);
		dct_compression_1.clk(clk);
		dct_compression_1.ready_o(ready);
		dct_compression_1.ready_i(ready_in);
		dct_compression_1.output(output);
		dct_compression_1.reset(reset);
	}

	~jpeg_enc() {}
};
