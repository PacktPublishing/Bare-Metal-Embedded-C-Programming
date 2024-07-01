#include "gpio.h"

int main(void)
{
	/*Initialize LED*/
	led_init();

	while(1)
	{
			led_on();
			for(int i = 0; i < 100000; i++){}

			led_off();
			for(int i = 0; i < 100000; i++){}

	}
}
