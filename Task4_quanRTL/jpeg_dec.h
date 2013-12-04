/*  jpeg_dec.h */
#include	"b2r.h"
#include	"idct_fix.h"
#include	"normalize.h"
#include	"zz_dec.h"
#include	"rl_dec.h"

#include	"P2FF.h"
#include    "unpack_data.h"


SC_MODULE(jpeg_dec) {

	/* in and output */
	sc_in<sc_uint<8> >	   input;
	sc_fifo_in<int>   parameters;
	sc_fifo_out<int>  output;

	sc_in<bool>			ready;
	sc_out<bool>		ask;
	sc_in_clk			clk;

	/* internal FIFOs */
	fifo_stat<int>*		rl_dec_out;
	fifo_stat<int>*		zz_dec_out;
	fifo_stat<float>*		norm_out;
	fifo_stat<int>*		idct_out;
	fifo_stat<sc_uint<8> >*		P2FF_out;
	fifo_stat<int>*     unpack_out;

	/* internal modules */
	unpack_data* unpack_data1;
	rl_dec* rl_dec_1;
	zz_dec* zz_dec_1;
	normalize* norm_1;
	idct_fix* idct_1;
	b2r* b2r_1;
	P2FF<sc_uint<8> > *P2FF_1;

	/* internal variables */
	int* quantization;
	int maxwidth;
	const char*	filename;


	// constructor
	jpeg_dec(sc_module_name name, int* _quantization, int _maxwidth, const char* _filename):
		sc_module(name),
		quantization(_quantization),
		maxwidth(_maxwidth),
		filename(_filename) {

		rl_dec_out = new fifo_stat<int>("rl_dec_out",1);
		zz_dec_out = new fifo_stat<int>("zz_dec_out",1);
		norm_out = new fifo_stat<float>("norm_out",1);
		idct_out = new fifo_stat<int>("idct_out",1);
		P2FF_out = new fifo_stat<sc_uint<8> >("P2FF_out",1);
		unpack_out = new fifo_stat<int>("unpack_out",1);


		P2FF_1 = new P2FF<sc_uint<8> >("P2FF_1");
		P2FF_1->input(input);
		P2FF_1->output(*P2FF_out);
		P2FF_1->ready(ready);
		P2FF_1->ask(ask);
		P2FF_1->clk(clk);

		unpack_data1 = new unpack_data("unpack_data1");
		unpack_data1->input(*P2FF_out);
		unpack_data1->output(*unpack_out);

		rl_dec_1 = new rl_dec("rl_dec_1");
		rl_dec_1->input(*unpack_out);
		rl_dec_1->output(*rl_dec_out);

		zz_dec_1 = new zz_dec("zz_dec_1");
		zz_dec_1->input(*rl_dec_out);
		zz_dec_1->output(*zz_dec_out);

		norm_1 = new normalize("norm_1", quantization);
		norm_1->input(*zz_dec_out);
		norm_1->output(*norm_out);

		idct_1 = new idct_fix("idct_1", filename);
		idct_1->input(*norm_out);
		idct_1->output(*idct_out);

		b2r_1 = new b2r("b2r_1",maxwidth);
		b2r_1->input(*idct_out);
		b2r_1->parameters(parameters);
		b2r_1->output(output);

	}

	// destructor
	~jpeg_dec() {
		delete b2r_1;
		delete idct_1;
		delete norm_1;
		delete zz_dec_1;
		delete rl_dec_1;
		delete rl_dec_out;
		delete zz_dec_out;
		delete norm_out;
		delete idct_out;
	}

};
