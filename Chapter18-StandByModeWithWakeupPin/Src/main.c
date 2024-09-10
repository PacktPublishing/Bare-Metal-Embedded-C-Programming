#include <stdio.h>
#include <string.h>
#include "standby_mode.h"
#include "gpio_exti.h"
#include "uart.h"



/*
 * To test the project, start by pressing the blue push-button to enter Standby mode.
 * Remember, PA0 is configured as the wake-up pin and is active low.
 * In normal mode, connect a jumper wire from PA0 to ground.
 * To trigger a wake-up event, pull out the jumper wire and connect it to 3.3V,
 * causing a change in logic that will wake the microcontroller from Standby mode.
 *
 * */

uint8_t g_btn_press;
static void check_reset_source(void);


int main(void)
{

	uart_init();

	wakeup_pin_init();

	/*Find reset source*/
    check_reset_source();

	/*Initialize EXTI*/
	pc13_exti_init();


	while(1)
	{

	}
}



static void check_reset_source(void)
{

	    /*Enable clock access to PWR*/
	     RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	  if ((PWR->CSR & PWR_CSR_SBF) == (PWR_CSR_SBF))
	  {
		  /*Clear Standby flag*/
		  PWR->CR |= PWR_CR_CSBF;

		  printf("System resume from Standby.....\n\r");

		  /*Wait for wakeup pin to be released*/
		  while(get_wakeup_pin_state() == 0){}

	  }

	  /*Check and Clear Wakeup flag*/
	  if((PWR->CSR & PWR_CSR_WUF) == PWR_CSR_WUF )
	  {
		  PWR->CR |= PWR_CR_CWUF;
	  }
}

static void exti_callback(void)
{
	standby_wakeup_pin_setup();


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
