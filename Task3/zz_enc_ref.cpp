#include "zz_enc_ref.h"

void zz_enc_ref::process() {

	int		i, j, k, l;
	int		temp_block[64];                     
	int		block[64];

	while(1) {
		//read in the blocks for 8 lines
	    for ( i = 0 ; i < 8 ; i ++) {
			for ( j = 0 ; j < 8 ; j++ ) {
        ask_i.write(true);
				wait();
				while (ready_i.read()!= true) wait();
				temp_block[8 * i + j ]= input.read();
        ask_i.write(false);
				wait();
			}
		}

		i = 0 , j = -1 , k = 0;

		for ( l = 0 ; l < 4 ; l++ ) {
			for ( j++ ; i >= 0 ; j++ , i-- ) {
				block[k] = temp_block[i*8+j];
				k++;
			}

			for ( i++ ; j >= 0 ; j-- , i++ ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
		}

		for ( l = 0 ; l < 3 ; l++ ) {
			for ( i-- , j += 2 ; j < 8 ; j++ , i-- ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
			for ( j-- , i += 2 ; i < 8 ; j-- , i++ ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
		}

		i-- , j += 2;
		block[k] = temp_block[i*8+j];

		for ( i = 0 ; i < 64 ; ++i ) {
      while (ask_o.read()!= true) wait();
			output.write (static_cast <sc_int<9> > (block[i]));
      ready_o.write(true);
			wait();
      ready_o.write(false);
      wait();
		}
	}
}

