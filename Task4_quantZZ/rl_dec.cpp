#include "rl_dec.h"

void rl_dec::process() {

	int		count;
	int		value;

	while (1) {
		// read and write DC value
		output.write(input.read());
		// process AC values
		count = 1;
		// read zero length value
		value = input.read();
		while (value != 63) {
			// process zl
			for ( ; value > 0 ; value-- ) {
				output.write(0);
				count++;
			}
 			// read and process non-zero value
			output.write(input.read());
			count ++;
			// read zero length value
			value = input.read();
		}
		// process zero length of 63
        for ( ; count<64 ; ) {
          output.write(0);
          count++;
        }
	}
	return;
}
