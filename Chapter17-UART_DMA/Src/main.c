#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "uart_dma.h"

extern uint8_t g_rx_cmplt;
extern uint8_t g_uart_cmplt;
extern uint8_t g_tx_cmplt;

extern char uart_data_buffer[UART_DATA_BUFF_SIZE];
char msg_buff[150] ={'\0'};


int main(void)
{

	uart2_rx_tx_init();
	dma1_init();
	dma1_stream5_uart_rx_config();
	sprintf(msg_buff,"Initialization...cmplt\n\r");
	dma1_stream6_uart_tx_config((uint32_t)msg_buff,strlen(msg_buff));

    while(!g_tx_cmplt){}


	while(1)
	{

		if(g_rx_cmplt)
		{
			sprintf(msg_buff, "Message received : %s \r\n",uart_data_buffer);
			g_rx_cmplt = 0;
			g_tx_cmplt = 0;
			g_uart_cmplt = 0;
			dma1_stream6_uart_tx_config((uint32_t)msg_buff,strlen(msg_buff));
		    while(!g_tx_cmplt){}

		}
	}
}

