#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"

uint8_t g_btn_press;

int main(void)
{

	/*Initialize debug UART*/
	uart_init();

	/*Initialize LED*/
	led_init();

	/*Initialize EXTI*/
	pc13_exti_init();


	while(1)
	{


	}
}


static void exti_callback(void)
{
	printf("BTN Pressed...\n\r");
	led_toggle();
}


void EXTI15_10_IRQHandler(void) {
	if((EXTI->PR & LINE13)!=0)
	{
		/*Clear PR flag*/
		EXTI->PR |=LINE13;

		//Do something...
		exti_callback();
	}

}
