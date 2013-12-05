#include "quant_zz_enc_RTL.h"

void quant_zz_enc_RTL::process_RTL() {

	// float	value;
  int temp_out,i,j,k;
  int value2; // temporary value
  float value3; // temporary value
  bool flag_t;
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
				count_j.write(0); //j
				count_i.write(0); //i
				count_k.write(0); //k
        flag.write(true);
				break;

	case READVALUE: // read value
		ready_o.write(false);
		if(ready_i.read() == true) {
      value3 = input.read();
      i = count_i.read();
      j = count_j.read();
      temp_out=(int)(floor(value3/quantization[i*8+j]+0.5));
      temp_block[i*8+j].write(temp_out);
			// value.write(temp_out); // read DC value
			ask_i.write(false);		
			state.write(WAITSAVE);
		}
		else {
			ask_i.write(true);
			state.write(READVALUE);
		}
		break;

	case WAITSAVE: //write value
         
		j = count_j.read();
    j++;
    if (j == 8)
    {
      count_j.write(0);
      i = count_i.read();
      i++;
      if(i == 8)
      {
        count_i.write(0);
        count_k.write(0);
        count_j.write(0);
        state.write(ZZREAD);
      }
      else
      {
        ask_i.write(true);
        ready_o.write(false);
        count_i.write(i);
        state.write(READVALUE);
      }
      
    }
    else
    {
      ask_i.write(true);
      ready_o.write(false);
      count_j.write(j);
      state.write(READVALUE);
    }
		break;

	case ZZREAD: //zig zag read
    if(count_k.read() >= 32)
    {
      count_i.write(0);
      count_k.write(0);
      count_j.write(0);
      flag.write(true);
      state.write(WRITEVALUES);
    }
    else if(count_i.read() < 0)
    {
      
      i = count_i.read();
      j = count_j.read();

      count_i.write(j);
      count_j.write(i+1);
      flag_t = !flag.read();
      flag.write(flag_t);
      state.write(ZZREAD);
    }
    else
    {
       
      j = count_j.read();
      i = count_i.read();
      k = count_k.read();

      if(flag.read())
      {
        block[k].write(temp_block[i*8+j].read());
        block[63-k].write(temp_block[((7-i)*8)+(7-j)].read());
      }
      else
      {
        value2 = temp_block[j*8+i].read();
        block[k].write(value2);
        value2 = temp_block[(7-j)*8+(7-i)].read();
        block[63-k].write(value2);
      } 
      k = k+1;
      count_k.write(k); 
      ++j;
      count_j.write(j);
      --i;
      count_i.write(i);
      state.write(ZZREAD);

    }
	
		break;

    case WRITEVALUES: // write values
      if(ask_o.read() == true) {
        k = count_k.read();
        
        output.write(block[k].read());
        k++;
        count_k.write(k);
        ready_o.write(true);
        count_i.write(0);
        state.write(WAITACDC);
        // state.write(TERMINATION);
      }
      else {
        ready_o.write(false);
        ask_i.write(false);		
        state.write(WRITEVALUES);
      }
      break;
      
    
    case WAITACDC: // write values
      ready_o.write(false);
      if(count_k.read()<63)
      {
        // ask_i.write(true);
        k = count_k.read();
        // k = count_k.read();
        // value.write(block[k].read());
        // ++k;
       
        
        if(block[k].read() == 0)
        {
          value2 = count_i.read();
          value2++;
          count_i.write(value2);
          state.write(WAITACDC);
         k++;
         count_k.write(k);
        }
        else
        {
          state.write(WRITECOUNT);
        }
       
        
        
      }
      else
      {
        count_k.write(0);
        count_i.write(0);
        ask_i.write(false);
        state.write(TERMINATION);
        // state.write(READVALUE);
      }
      break;  
      
    case WRITECOUNT:
      if(ask_o.read() == true)
      {
        output.write(static_cast <sc_int<9> >(count_i.read()));
        ready_o.write(true);
        state.write(WAITCOUNT);
      }
      else
      {
        ask_i.write(false);
        ready_o.write(false);
        state.write(WRITECOUNT);
      }
      break;
      
    
    case WAITCOUNT:
      ready_o.write(false);
      state.write(WRITEVALUES);
      break;  
          
    case TERMINATION: // write values
      if(ask_o.read() == true)
				{
					output.write(63);
					ask_i.write(true);
					ready_o.write(true);
          count_j.write(0); //j
          count_i.write(0); //i
          count_k.write(0); //k
					state.write(READVALUE);
				}
				break;
        

    } // end switch
  }// end if

}