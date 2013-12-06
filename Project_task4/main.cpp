#define		SC_INCLUDE_FX
#include	<systemc.h>
#include	"fifo_stat.h"
#include	"my_fxtype_params.h"
#include	"fix_stat.h"

#include	"test.h"
#include	"df_fork.h"
#include	"snk.h"
#include	"src.h"

#include	"jpeg_dec.h"
//#include	"jpeg_enc.h"
#include	"jpeg_enc_float.h"

#define MAXWIDTH 1024
#define MAXWIDTH8 ((MAXWIDTH +7)/8 * 8)

int sc_main (int argc , char *argv[])
{
	/*
	int	quantization[64] = { 8,  6,  6,  7,  6,  5,  8,  7,
							 7,  7,  9,  9,  8, 10, 12, 20,
							13, 12, 11, 11, 12, 25, 18, 19,
							15, 20, 29, 26, 31, 30, 29, 26,
							28, 28, 32, 36, 46, 39, 32, 34,
							44, 35, 28, 28, 40, 55, 41, 44,
							48, 49, 52, 52, 52, 31, 39, 57,
							61, 56, 50, 60, 46, 51, 52, 50 };
	 */

	int	quantization[64] = {16, 11, 10, 16, 24, 40, 51, 61,
							12, 12, 14, 19, 26, 58, 60, 55,
							14, 13, 16, 24, 40, 57, 69, 56,
							14, 17, 22, 29, 51, 87, 80, 62,
							18, 22, 37, 56, 68,109,103, 77,
							24, 35, 55, 64, 81,104,113, 92,
							49, 64, 78, 87,103,121,120,101,
							72, 92, 95, 98,112,100,103, 99};


	sc_ufixed<14,-2,SC_TRN,SC_WRAP> quantization_i[64] = {
			0.0625000000000000, 0.0909090909090909, 0.100000000000000, 0.0625000000000000, 0.0416666666666667, 0.0250000000000000, 0.0196078431372549, 0.0163934426229508,
			0.0833333333333333, 0.0833333333333333, 0.0714285714285714, 0.0526315789473684, 0.0384615384615385, 0.0172413793103448, 0.0166666666666667, 0.0181818181818182,
			0.0714285714285710, 0.0769230769230769, 0.0625000000000000, 0.0416666666666667, 0.0250000000000000, 0.0175438596491228, 0.0144927536231884, 0.0178571428571429,
			0.0714285714285714, 0.0588235294117647, 0.0454545454545455, 0.0344827586206897, 0.0196078431372549, 0.0114942528735632, 0.0125000000000000, 0.0161290322580645,
			0.0555555555555560, 0.0454545454545455, 0.0270270270270270, 0.0178571428571429, 0.0147058823529412, 0.00917431192660551, 0.00970873786407767, 0.0129870129870130,
			0.0416666666666667, 0.0285714285714286, 0.0181818181818182, 0.0156250000000000, 0.0123456790123457, 0.00961538461538462, 0.00884955752212389, 0.0108695652173913,
			0.0204081632653060, 0.0156250000000000, 0.0128205128205128, 0.0114942528735632, 0.00970873786407767, 0.00826446280991736, 0.00833333333333333, 0.00990099009900990,
			0.0138888888888889, 0.0108695652173913, 0.0105263157894737, 0.0102040816326531, 0.00892857142857143, 0.0100000000000000, 0.00970873786407767, 0.0101010101010101
	};

	// definition of default files
	const char*	inputfile		= "datain.pgm";
	const char*	outputfile		= "dataout.pgm";
	const char* typefile 		= "types.txt";

	// definition of FIFO queues
	fifo_stat<int>	stimulus("stimulus",1);
	fifo_stat<int>	parameters("parameters",3);
	fifo_stat<int>	stimulus_dup1("stimulus_dup1",1);
	fifo_stat<int>	stimulus_dup2("stimulus_dup2",MAXWIDTH8*8+64+64+64+64+64);
	fifo_stat<int>	parameters_dup1("parameters_dup1",3);
	fifo_stat<int>	parameters_dup2("parameters_dup2",3);
	fifo_stat<int>	parameters_dup3("parameters_dup3",3);
	fifo_stat<int>	jpeg_dec_in("jpeg_dec_in",1);
	fifo_stat<int>	result("result",1);
	fifo_stat<int>	result_dup1("result_dup1",1);
	fifo_stat<int>	result_dup2("result_dup2",1);

	// definition of signals
	sc_clock 				clk("clock", 12, SC_NS);
	sc_signal<bool> 		ask;
	sc_signal<bool>			ready;
	sc_signal< sc_uint<8> > data;

	// processing of command-line arguments
	bool	detected;
	for(int i=3; i<=argc; i+=2) {
		cout << argv[i-2] << " " << argv[i-1] << endl;
		detected = 0;
		if (strcmp(argv[i-2],"-i")==0) {
			inputfile = argv[i-1];
			detected = 1;
		}
		if (strcmp(argv[i-2],"-o")==0) {
			outputfile = argv[i-1];
			detected = 1;
		}
		if (strcmp(argv[i-2],"-t")==0) {
			typefile = argv[i-1];
			detected = 1;
		}
		if (detected == 0) {
			cout << "option " << argv[i-2] << " not known " << endl;
		}
	}

	//	definition of modules
	src src1("src1", inputfile, MAXWIDTH);
	src1.output(stimulus);
	src1.parameters(parameters);

	df_fork<int,2> fork1("fork1");
	fork1.in(stimulus);
	fork1.out[0](stimulus_dup1);
	fork1.out[1](stimulus_dup2);

	df_fork<int,3> fork_param("fork_param");
	fork_param.in(parameters);
	fork_param.out[0](parameters_dup1);
	fork_param.out[1](parameters_dup2);
	fork_param.out[2](parameters_dup3);

//	jpeg_enc jpeg_enc_1("jpeg_enc_1", quantization, MAXWIDTH);
//	jpeg_enc_1.input(stimulus_dup1);
//	jpeg_enc_1.clk(clk);
//	jpeg_enc_1.parameters(parameters_dup1);
//	jpeg_enc_1.output(data);
//	jpeg_enc_1.ready(ready);
//	jpeg_enc_1.ask(ask);

	jpeg_enc_float jpeg_enc_1("jpeg_enc_1", quantization_i, MAXWIDTH);
	jpeg_enc_1.input(stimulus_dup1);
	jpeg_enc_1.clk(clk);
	jpeg_enc_1.parameters(parameters_dup1);
	jpeg_enc_1.output(data);
	jpeg_enc_1.ready(ready);
	jpeg_enc_1.ask(ask);

	jpeg_dec jpeg_dec_1("jpeg_dec_1", quantization, MAXWIDTH, typefile);
	jpeg_dec_1.input(data);
	jpeg_dec_1.clk(clk);
	jpeg_dec_1.ready(ready);
	jpeg_dec_1.ask(ask);
	jpeg_dec_1.parameters(parameters_dup2);
	jpeg_dec_1.output(result);

	df_fork<int,2> fork2("fork2");
	fork2.in(result);
	fork2.out[0](result_dup1);
	fork2.out[1](result_dup2);

	snk snk1("snk1", outputfile);
	snk1.input(result_dup1);
	snk1.parameters(parameters_dup3);

	test test1("test1");
	test1.reference(stimulus_dup2);
	test1.data(result_dup2);

	sc_start();

	return 0;
}
