#include "rl_enc_pack.h"

void rl_enc_pack::process() {

	sc_int<9>	count;
	int		k;
	int 		in_read;
	sc_int<9>	value;
	//int value,count;

	while (1)
	{
		// read and write DC value
		in_read = input.read();
		value = static_cast <sc_int<9> >(in_read);
		//output.write(value);
		output.write(value);
		count = 0;

		for( k = 1 ; k < 64 ; k++ ) {
			in_read = input.read();
			value = static_cast <sc_int<9> >(in_read);
			//value = input.read();
			if ( value == 0 ) {
				count++;
			}
			else {
				output.write(count);
				count = 0;
				output.write(value);
			}
		}
		output.write(63);
	}
}
