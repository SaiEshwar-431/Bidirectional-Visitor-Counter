#include "types.h"


void delay_ms(u32 dlyMS)
{
	u32 j;
	for(;dlyMS>0;dlyMS--)
	{
		for(j=12000;j>0;j--);
	}
}
