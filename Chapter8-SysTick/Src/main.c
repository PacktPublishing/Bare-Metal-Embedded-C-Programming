#include "gpio.h"
#include "systick.h"

int main(void)
{
	/*Initialize LED*/
	led_init();


	while(1){

		/*Delay for 500ms*/
		systick_msec_delay(500);
		led_toggle();


	}
}
