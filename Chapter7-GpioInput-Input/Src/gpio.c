#include "gpio.h"

#define GPIOAEN			(1U<<0)
#define GPIOCEN		(1U<<2)

#define LED_BS5			(1U<<5)  /*Bit Set Pin 5*/
#define LED_BR5			(1U<<21) /*Bit Reset Pin 5*/
#define BTN_PIN		(1U<<13)

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



void button_init(void)
{
	/*Enable clock access to PORTC*/
	RCC->AHB1ENR |=GPIOCEN;

	/*Set PC13 as an input pin*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

}


bool get_btn_state(void)
{

	/*Note : BTN is active low*/

	/*Check if button is pressed*/
	if(GPIOC->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}

}
