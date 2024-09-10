#include "adc.h"

#define GPIOAEN		(1U<<0)
#define ADC1EN		(1U<<8)
#define ADC_CH1		(1U<<0)
#define ADC_SEQ_LEN_1  0x00
#define CR2_ADCON	 (1U<<0)
#define CR2_CONT	 (1U<<1)
#define CR2_SWSTART	  (1U<<30)
#define SR_EOC		  (1U<<1)

void pa1_adc_init(void)
{
	/****Configure the ADC GPIO Pin**/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA1 mode to analog mode*/
	GPIOA->MODER |=(1U<<2);
	GPIOA->MODER |=(1U<<3);

	/****Configure the ADC Module**/
	/*Enable clock access to the ADC module*/
	RCC->APB2ENR |=ADC1EN;

	/*Set conversion sequence start*/
	ADC1->SQR3 = ADC_CH1;

	/*Set conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*Enable ADC module*/
	ADC1->CR2 |=CR2_ADCON;

}


void start_conversion(void)
{
	/*Enable continuous conversion*/
	ADC1->CR2 |=CR2_CONT;

	/*Start ADC conversion*/
	ADC1->CR2 |=CR2_SWSTART;
}


uint32_t adc_read(void)
{
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)){}

	/*Read converted value*/
	return (ADC1->DR);
}
