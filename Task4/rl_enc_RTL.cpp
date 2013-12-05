#include "rl_enc_RTL.h"

void rl_enc_RTL::process()
{
	sc_uint<7>	value2;

	if (reset.read() == false)
	{
		ask_i.write(false);
		ready_o.write(false);
		state.write(RESET);
	}
	else
	{
		switch (state.read())
		{
			case RESET:
				ask_i.write(true);
				ready_o.write(false);
				state.write(READACDC);
				break;

			case READACDC:
				ready_o.write(false);
				k.write(1);
				if(ready_i.read() == true)
				{
					value.write(input.read());
					ask_i.write(false);
					state.write(WRITEACDC);
				}
				else
				{
					ask_i.write(true);
					state.write(READACDC);
				}
				break;

			case WRITEACDC:
				if(ask_o.read() == true)
				{
					count.write(0);
					output.write(value.read());
					ready_o.write(true);
					state.write(WAITACDC);
				}
				else
				{
					ask_i.write(false);
					ready_o.write(false);
					state.write(WRITEACDC);
				}
				break;

			case WAITACDC:
				ready_o.write(false);
				if(k.read() < 64)
				{
					ask_i.write(true);
					value2 = k.read();
					value2++;
					k.write(value2);
					state.write(READZERO);
				}
				else
				{
					ask_i.write(false);
					state.write(TERMINATION);
				}
				break;

			case READZERO:
				ready_o.write(false);
				if(ready_i.read() == true)
				{
					value.write(input.read());
					ask_i.write(false);
					state.write(CHECKZERO);
				}
				break;

			case CHECKZERO:
				if(value.read() == 0)
				{
					value2 = count.read();
					value2++;
					count.write(value2);
					state.write(WAITACDC);
				}
				else
				{
					state.write(WRITECOUNT);
				}
				break;

			case WRITECOUNT:
				if(ask_o.read() == true)
				{
					output.write(static_cast <sc_int<9> >(count.read()));
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
				state.write(WRITEACDC);
				break;

			case TERMINATION:
				if(ask_o.read() == true)
				{
					output.write(63);
					ask_i.write(true);
					ready_o.write(true);
					state.write(READACDC);
				}
				break;
		}
	}
}
