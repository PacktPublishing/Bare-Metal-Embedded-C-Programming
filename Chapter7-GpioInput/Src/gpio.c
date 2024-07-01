#include "gpio.h"

#define GPIOAEN			(1U<<0)
#define LED_BS5			(1U<<5)  /*Bit Set Pin 5*/
#define LED_BR5			(1U<<21) /*Bit Reset Pin 5*/

void led_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 mode to output mode*/
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);
}


void led_on(void)
{
	/*Set PA5 high*/
	GPIOA->BSRR |=LED_BS5;
}

void led_off(void)
{
	/*Set PA5 high*/
	GPIOA->BSRR |=LED_BR5;

}
