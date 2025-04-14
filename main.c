
#include <stdio.h>
#include "pico/stdlib.h"

//------------------------------------------------------------------------------

int main()
{
	stdio_init_all();

	absolute_time_t t;
	t = make_timeout_time_ms(500);
	while (true) {
		if(time_reached(t)) {
			t = make_timeout_time_ms(500);
			printf("\r\nMDT01>");
		}
	}
}

//------------------------------------------------------------------------------

