#include "quantize_RTL.h"

void quantize_RTL::process_RTL() {

	// float	value;
  int temp_out,i,j;
  int value2; // temporary value
  float value3; // temporary value

	if (reset.read() == false) {
		state.write(RESET);
		ready_o.write(false);
		ask_i.write(false);
	} else {
		switch (state.read()) {

			case RESET: // reset cycle
				ask_i.write(true);		//indicate that we want to read in next cycle
				ready_o.write(false);		//indicate that we do not write data in this cycle
				state.write(READVALUE);
				count_1.write(0);
				count_2.write(0);
				break;

	case READVALUE: // read value
		ready_o.write(false);
		if(ready_i.read() == true) {
      value3 = input.read();
      i = count_2.read();
      j = count_1.read();
      temp_out=(int)(floor(value3/quantization[i*8+j]+0.5));
			value.write(temp_out); // read DC value
			ask_i.write(false);		
			state.write(WRITEVALUE);
		}
		else {
			ask_i.write(true);
			state.write(READVALUE);
		}
		break;

	case WRITEVALUE: //write value
		if(ask_o.read() == true) {
			output.write(value.read()); // write AC/DC value
			value2 = count_1.read();
			value2++;
      ask_i.write(false);
			ready_o.write(true); //indicate that we do write data in this cycle
      if (value2 == 8)
      {
        count_1.write(0);
        value2 = count_2.read();
        value2++;
        if(value2 == 8)
        {
          count_2.write(0);
          state.write(TERMINATION);
        }
        else
        {
          count_2.write(value2);
          state.write(WAITWRITE);
        }
        
      }
      else
      {
        count_1.write(value2);
        state.write(WAITWRITE);
      }
			
		}
		else {
			ready_o.write(false);
			ask_i.write(false);		
			state.write(WRITEVALUE);
		}
		break;

	case WAITWRITE: //wait for succesfull write value
		ask_i.write(true);
		ready_o.write(false);
		state.write(READVALUE);
		break;

	case TERMINATION: // finish
		ready_o.write(false);
    count_1.write(0);
    count_2.write(0);
    state.write(READVALUE);
		break;



	} // end switch
  }// end if

}