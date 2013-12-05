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

// #include	"quantize_ref.h"
// #include	"quantize_RTL.h"
// #include	"zz_enc_ref.h"
// #include "quant_zz_RTL.h"
// #include "quant_zz_enc_RTL.h"
#include "dct_compression_RTL.h"
// #include	"rl_enc_ref.h"
// #include	"rl_enc_RTL.h"
#include 	"FF2P.h"
// #include 	"pack_data.h"
// #include 	"pack_data_RTL.h"

SC_MODULE(dct_compressor) {


  sc_in<sc_fixed<22,12,SC_TRN,SC_WRAP> >  input;
  sc_out<sc_uint<8> >   output;
  sc_in<bool>  ask;
  sc_out<bool>  ask_in;
  sc_in<bool>  clk;
  sc_out<bool>  ready;
  sc_in<bool>  ready_in;

  
  sc_signal <bool > ask_zz;
  sc_signal <bool > ready_zz;  
  sc_signal <bool > ask_rl;
  sc_signal <bool > ready_rl;
  sc_signal <bool > ask_pack;
  sc_signal <bool > ready_pack;
  sc_signal <bool > reset;
  sc_signal<sc_int<9> >	quant_out;
  // sc_signal<sc_int<9> >	zz_enc_out;
  sc_signal<sc_int<9> >	rl_enc_out;
  
  // int* quantization;

  // fifo_stat<sc_int<9> >	quant_out;
  // fifo_stat<sc_int<9> >	zz_enc_out;
  // fifo_stat<sc_int<9> >	rl_enc_out;
  // fifo_stat<sc_uint<8> >pack_out;

  // quantize_ref quant_1;
  // quantize_RTL quant_1;
  // quant_zz_RTL quant_1;
  dct_compression_RTL quant_1;
  // quant_zz_enc_RTL quant_1;
  // zz_enc_ref zz_enc_1;
  // rl_enc_ref rl_enc_1;
  // rl_enc_RTL rl_enc_1;
  // pack_data_RTL pack_data1;
  // pack_data pack_data1;

  // FF2P <sc_uint<8> > FF2P_1;

  // SC_HAS_PROCESS(dct_compressor);

  dct_compressor(sc_module_name name, int* _quantization):
		sc_module(name),
    // quant_out("quant_out",1),
		// zz_enc_out("zz_enc_out",1),
    // rl_enc_out("rl_enc_out",1),
		quant_1("quant_1",_quantization){

      reset.write(true);
      quant_1.input(input);
      quant_1.ask_i(ask_in);
      quant_1.ready_i(ready_in);
      quant_1.ask_o(ask);
      quant_1.ready_o(ready);
      quant_1.clk(clk);
      quant_1.reset(reset);
      quant_1.output(output);

      // zz_enc_1.input(quant_out);
      // zz_enc_1.ask_i(ask_zz);
      // zz_enc_1.ready_i(ready_zz);
      // zz_enc_1.ask_o(ask_rl);
      // zz_enc_1.ready_o(ready_rl);
      // zz_enc_1.clk(clk);
      // zz_enc_1.output(zz_enc_out);

      // rl_enc_1.input(quant_out);
      // rl_enc_1.reset(reset);
      // rl_enc_1.ask_i(ask_zz);
      // rl_enc_1.ready_i(ready_zz);
      // rl_enc_1.ask_o(ask_pack);
      // rl_enc_1.ready_o(ready_pack);
      // rl_enc_1.clk(clk);
      // rl_enc_1.output(rl_enc_out);

      // pack_data1.input(quant_out);
      // pack_data1.reset(reset);
      // pack_data1.ask_i(ask_zz);
      // pack_data1.ready_i(ready_zz);
      // pack_data1.ask_o(ask);
      // pack_data1.ready_o(ready);
      // pack_data1.clk(clk);
      // pack_data1.output(output);

      // FF2P_1.input(pack_out);
      // FF2P_1.ask(ask);
      // FF2P_1.ready(ready);
      // FF2P_1.clk(clk);
      // FF2P_1.output(output);
		}

  void process();
};

#endif
