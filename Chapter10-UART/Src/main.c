#include <stdio.h>
#include "uart.h"


int main(void)
{

	/*Initialize debug UART*/
	uart_init();


	while(1)
	{

		printf("Hello from STM32...\r\n");

	}
}
