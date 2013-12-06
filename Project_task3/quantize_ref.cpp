#include "quantize_ref.h"

void quantize_ref::process() {

	int		i, j;
	float	value;
	int		temp_out;
  ready_o.write(false);
	while (1) {
		for(i=0;i<8;i++) {
			for (j=0; j<8;j++){
				ask_i.write(true);
        
				wait();
				while (ready_i.read()!= true) wait();
				value = input.read();
				temp_out=(int)(floor(value/quantization[i*8+j]+0.5));
				ask_i.write(false);
				wait();
//				cout << temp_out << " ";
				//wait for the ask of the next block
				// ready_o.write(false);
				while (ask_o.read()!= true) wait();
				output.write(static_cast <sc_int <9> >(temp_out));
				ready_o.write(true);
				wait();
        ready_o.write(false);

			}
//			cout << endl;
		}
	}
}

