#include "dct_compression_RTL_float.h"

void dct_compression_RTL_float::process_RTL()
{
	sc_int<9>	temp_out;
	sc_int<5>	i; // can be improved to avoid getting negative
	sc_uint<4>	j;
	sc_uint<7>	k;
	sc_int<9>	value2; // temporary value
	sc_fixed<22,12,SC_TRN,SC_WRAP>	value3; // temporary value
	bool 		flag_t;
	sc_int<9> 	firstByte_temp;
	bool		rem;

	if (reset.read() == true)
	{
		state.write(RESET);
		ready_o.write(false);
		ask_i.write(false);
	}
	else
	{
		switch (state.read())
		{
			case RESET: // reset cycle
			{
				ask_i.write(true);		//indicate that we want to read in next cycle
				ready_o.write(false);		//indicate that we do not write data in this cycle
				state.write(READVALUE);
				count_j.write(0); //j
				count_i.write(0); //i
				count_k.write(0); //k
				flag.write(true);
				break;
			}

			case READVALUE: // read value
			{
				ready_o.write(false);
				if(ready_i.read() == true)
				{
					
					i = count_i.read();
					j = count_j.read();
					// temp_out = (int)(floor(value3*quantization_i[(i<<3)+j]+0.5));
          
          quantization_temp = quantization_i[(i<<3)+j];
          ask_i.write(false);
          state.write(MULTIPLIER);
          
					// temp_out = (int)(floor(value3*quantization_i[(i<<3)+j]+0.5));
					// temp_block[(i<<3)+j].write(temp_out);
					
					// state.write(WAITSAVE);
				}
				else
				{
					ask_i.write(true);
					state.write(READVALUE);
				}
				break;
			}
      
      case MULTIPLIER:
      {
        i = count_i.read();
				j = count_j.read();
        value3 = input.read();
        temp_out = (int)(floor(value3*quantization_temp+0.5));
				temp_block[(i<<3)+j].write(temp_out);
        ask_i.write(false);
				state.write(WAITSAVE);
        break;
      }
      

			case WAITSAVE: //write value
			{
				j = count_j.read();
				//j++;
				if (j == 7)
				{
					count_j.write(0);
					i = count_i.read();
					//i++;
					if(i == 7)
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
            i++;
						count_i.write(i);
						state.write(READVALUE);
					}

				}
				else
				{
					ask_i.write(true);
					ready_o.write(false);
          j++;
					count_j.write(j);
					state.write(READVALUE);
				}
				break;
			}

			case ZZREAD: //zig zag read
			{
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

					count_i.write(static_cast < sc_int<7> >(j));
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
						block[k].write(temp_block[(i<<3)+j].read());
						block[63-k].write(temp_block[((7-i)<<3)+(7-j)].read());
					}
					else
					{
						block[k].write(temp_block[(j<<3)+i].read());
						block[63-k].write(temp_block[((7-j)<<3)+(7-i)].read());
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
			}

			case WRITEVALUES: // write values
			{
				if(ask_o.read() == true)
				{
					k = count_k.read();

					firstByte.write( block[k].read());
					firstByte_temp = block[k].read();
					output.write(static_cast < sc_uint<8> >(firstByte_temp.range(8,1)));

					k++;
					count_k.write(k);
					ready_o.write(true);
					count_i.write(0);
					state.write(WAITACDC);
				}
				else
				{
					ready_o.write(false);
					ask_i.write(false);
					state.write(WRITEVALUES);
				}
				break;
			}

			case WAITACDC: // write values
			{
				ready_o.write(false);
				if(count_k.read() < 64)
				{
					k = count_k.read();

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
				}
				break;
			}

			case WRITECOUNT:
			{
				if(ask_o.read() == true)
				{
					firstByte_temp = firstByte;
					rem = firstByte_temp[0];
					firstByte_temp = 0;
					firstByte_temp[7] = rem;
					firstByte_temp.range(6,0) = count_i.read();

					output.write(static_cast <sc_uint<8> >(firstByte_temp.range(7,0)));
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
			}

			case WAITCOUNT:
			{
				ready_o.write(false);
				state.write(WRITEVALUES);
				break;
			}

			case TERMINATION: // write values
			{
				if(ask_o.read() == true)
				{
					firstByte_temp = firstByte;
					rem = firstByte_temp[0];
					firstByte_temp = 0;
					firstByte_temp[7] = rem;
					firstByte_temp.range(6,0) = 63;

					output.write(static_cast <sc_uint<8> >(firstByte_temp.range(7,0)));
					ask_i.write(true);
					ready_o.write(true);
					count_j.write(0); //j
					count_i.write(0); //i
					count_k.write(0); //k
					state.write(READVALUE);
				}
				break;
			}
		} // end switch
	}// end if
}
