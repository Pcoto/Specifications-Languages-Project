#include "rl_enc_ref.h"

void rl_enc_ref::process() {

	sc_int<9>	count;
	int		k;
	int 		in_read;
	sc_int<9>	value;
	//int value,count;
	ready_o.write(false);
	while (1)
	{
		// read and write DC value
		ask_i.write(true);
		wait();
		while (ready_i.read()!= true) wait();
		in_read = input.read();
		value = static_cast <sc_int<9> >(in_read);
		//output.write(value);
		ask_i.write(false);
		wait();
		//wait for the ask of the next block
		while (ask_o.read()!= true) wait();
		output.write(value);
		ready_o.write(true);
		wait();
		ready_o.write(false);
		count = 0;
    wait();


		for( k = 1 ; k < 64 ; k++ ) {
			ask_i.write(true);
			wait();
			while (ready_i.read()!= true) wait();
			in_read = input.read();
			value = static_cast <sc_int<9> >(in_read);
			//value = input.read();
			ask_i.write(false);
			wait();
			if ( value == 0 ) {
				count++;
			}
			else {
				while (ask_o.read()!= true) wait();
				output.write(count);
				ready_o.write(true);
				wait();
				ready_o.write(false);
        wait();
				count = 0;
				while (ask_o.read()!= true) wait();
				output.write(value);
				ready_o.write(true);
				wait();
				ready_o.write(false);
        wait();
       
			}
		}
    
		while (ask_o.read()!= true) wait();
		output.write(63);
		ready_o.write(true);
		wait();
		ready_o.write(false);
    wait();
	}
}
