#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "gpio.h"
#include "iwdg.h"
#include "gpio_exti.h"

uint8_t g_btn_press;
static void check_reset_source(void);

int main(void)
{

	/*Initialize debug UART*/
	uart_init();

	/*Initialize LED*/
	led_init();

	/*Initialize EXTI*/
	pc13_exti_init();

	/*Find reset source*/
    check_reset_source();

	/*Initialize IWDG*/
	iwdg_init();



	while(1)
	{

        if( g_btn_press != 1)
         {
        	  /*Refresh IWDG down-counter to default value*/
        	 IWDG->KR = IWDG_KEY_RELOAD;
        	 led_toggle();
        	 for(int i = 0; i < 90000; i++){}
         }
	}
}


static void check_reset_source(void)
{
	  if ((RCC->CSR & RCC_CSR_IWDGRSTF) == (RCC_CSR_IWDGRSTF))
	  {
		  /*Clear IWDG Reset flag*/
		  RCC->CSR = RCC_CSR_RMVF;

		  /*Turn LED On*/
		  led_on();
		  printf("RESET was caused by IWDG.....\n\r");

		  while( g_btn_press != 1)
		  {

		  }

		  g_btn_press =  0;

	  }
}

static void exti_callback(void)
{
	g_btn_press = 1;

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
