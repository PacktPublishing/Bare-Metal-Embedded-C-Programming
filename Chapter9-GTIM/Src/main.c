#include "gpio.h"
#include "tim.h"

int main(void)
{
	/*Initialize LED*/
	led_init();

    /*Initialize timer*/
	tim2_1hz_init();

	while(1)
	{
			led_toggle();

	        /*Wait for UIF */
			while(!(TIM2->SR & SR_UIF)){}

			/*Clear UIF*/
			TIM2->SR &=~SR_UIF;



	}
}
