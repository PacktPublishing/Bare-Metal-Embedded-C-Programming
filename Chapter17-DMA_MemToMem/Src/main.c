#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "dma.h"
#include "uart.h"


#define BUFFER_SIZE		5


uint16_t sensor_data_arr[BUFFER_SIZE] = {892,731,1234,90,23};
uint16_t temp_data_arr[BUFFER_SIZE];

volatile uint8_t g_transfer_cmplt;

int main(void)
{
	g_transfer_cmplt = 0;

	uart_init();
	dma2_mem2mem_config();

	dma_transfer_start((uint32_t)sensor_data_arr,(uint32_t) temp_data_arr, BUFFER_SIZE);

	/*Wait until transfer complete*/
	while(!g_transfer_cmplt){}

	for( int i = 0; i < BUFFER_SIZE; i++)
	{
		printf("Temp buffer[%d]: %d\r\n",i,temp_data_arr[i]);
	}

	g_transfer_cmplt = 0;


	while(1)
	{

	}
}


void DMA2_Stream0_IRQHandler(void)
{

	/*Check if transfer complete interrupt occurred*/
	if((DMA2->LISR) & LISR_TCIF0)
	{
		g_transfer_cmplt = 1;

		/*Clear flag*/
		DMA2->LIFCR |=LIFCR_CTCIF0;
	}

	/*Check if transfer error occurred*/
	if((DMA2->LISR) & LISR_TEIF0)
	{
		/*Do something...*/
		/*Clear flag*/
		DMA2->LIFCR |= LIFCR_CTEIF0;

	}

}
