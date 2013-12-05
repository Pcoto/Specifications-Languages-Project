#include "unpack_data.h"

void unpack_data::unpack()
{
	while(1)
	{
		offset = 0;
		rl = 0;

		while(rl != 63)
		{
			/*====================== VALUE ======================*/
			if(offset == 0)
			{
				in_byte = input.read();
			}

			int range_start, range_end;

			int mul = (offset+VALUE_BITS) / 8;
			int rem = (offset+VALUE_BITS) % 8;

			if (mul > 0)
			{
				range_start = VALUE_BITS-1;
				range_end   = VALUE_BITS  - (8-offset);
				value.range(range_start, range_end) = in_byte.range(7-offset, 0);

				for (int i = 1; i < mul; i++)
				{
					in_byte = input.read();

					range_start = VALUE_BITS-1 - (8*i	 -offset);
					range_end   = VALUE_BITS   - (8*(i+1)-offset);
					value.range(range_start, range_end) = in_byte;
				}

				if(rem != 0)
				{
					in_byte = input.read();
					value.range(rem-1, 0) = in_byte(7, 8-rem);
				}
			}
			else
			{
				value = in_byte.range(7-offset, 8-rem);
			}

			output.write(value);

			offset = rem;

			/*====================== RL ======================*/
			if(offset == 0)
			{
				in_byte = input.read();
			}

			mul = (offset+RL_BITS) / 8;
			rem = (offset+RL_BITS) % 8;

			if (mul > 0)
			{
				range_start = RL_BITS-1;
				range_end   = RL_BITS - (8-offset);
				rl.range(range_start, range_end) = in_byte.range(7-offset, 0);

				for (int i = 1; i < mul; i++)
				{
					in_byte = input.read();

					range_start = RL_BITS-1 - (8*i	 -offset);
					range_end   = RL_BITS   - (8*(i+1)-offset);
					rl.range(range_start, range_end) = in_byte;
				}

				if(rem != 0)
				{
					in_byte = input.read();
					rl.range(rem-1, 0) = in_byte(7, 8-rem);
				}
			}
			else
			{
				rl = in_byte.range(7-offset, 8-rem);
			}

			output.write(rl);

			offset = rem;
		}
	}
}
