/*
 * dct_compression.h
 *
 *  Created on: 04/dic/2013
 *      Author: giovanni
 */

/*  DCT_compressor.h */
#ifndef _DCT_compressor
#define _DCT_compressor
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

#include	"quantize_ref.h"
#include	"zz_enc_ref.h"
#include	"rl_enc_ref.h"
#include 	"FF2P.h"
#include 	"pack_data.h"

SC_MODULE(dct_compressor) {


  my_fifo_in<sc_fixed<22,12,SC_TRN,SC_WRAP> >  input;
  sc_out<sc_uint<8> >   output;
  sc_in<bool>  ask;
  sc_in<bool>  clk;
  sc_out<bool>  ready;

  // int* quantization;

  fifo_stat<sc_int<9> >	quant_out;
  fifo_stat<sc_int<9> >	zz_enc_out;
  fifo_stat<sc_int<9> >	rl_enc_out;
  fifo_stat<sc_uint<8> >pack_out;

  quantize_ref quant_1;
  zz_enc_ref zz_enc_1;
  rl_enc_ref rl_enc_1;
  pack_data pack_data1;

  FF2P <sc_uint<8> > FF2P_1;

  // SC_HAS_PROCESS(dct_compressor);

  dct_compressor(sc_module_name name, int* _quantization):
		sc_module(name),
    quant_out("quant_out",1),
		zz_enc_out("zz_enc_out",1),
    rl_enc_out("rl_enc_out",1),
		quant_1("quant_1",_quantization),
		zz_enc_1("zz_enc_1"),
		rl_enc_1("rl_enc_1"),
		pack_data1("pack_data1"),
	FF2P_1("FF2P_1"){

      quant_1.input(input);
      quant_1.output(quant_out);

      zz_enc_1.input(quant_out);
      zz_enc_1.output(zz_enc_out);

      rl_enc_1.input(zz_enc_out);
      rl_enc_1.output(rl_enc_out);

      pack_data1.input(rl_enc_out);
      pack_data1.output(pack_out);

      FF2P_1.input(pack_out);
      FF2P_1.ask(ask);
      FF2P_1.ready(ready);
      FF2P_1.clk(clk);
      FF2P_1.output(output);
		}

  void process();
};

#endif
