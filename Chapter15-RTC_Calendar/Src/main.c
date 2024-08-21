#include <stdio.h>
#include "rtc.h"
#include "uart.h"


#define BUFF_LEN		20

uint8_t time_buff[BUFF_LEN] = {0};
uint8_t date_buff[BUFF_LEN] = {0};

static void display_rtc_calendar(void);

int main(void)
{

	/*Initialize debug UART*/
	uart_init();

	/*Initialize rtc*/
	rtc_init();

	while(1)
	{
		display_rtc_calendar();

	}
}

static void display_rtc_calendar(void)
{
	/*Display format :  hh : mm : ss*/
	sprintf((char *)time_buff,"%.2d :%.2d :%.2d",rtc_convert_bcd2dec(rtc_time_get_hour()),
			rtc_convert_bcd2dec(rtc_time_get_minute()),
			rtc_convert_bcd2dec(rtc_time_get_second()));

	printf("Time : %.2d :%.2d :%.2d\n\r",rtc_convert_bcd2dec(rtc_time_get_hour()),
			rtc_convert_bcd2dec(rtc_time_get_minute()),
			rtc_convert_bcd2dec(rtc_time_get_second()));


	/*Display format :  mm : dd : yy*/
	sprintf((char *)date_buff,"%.2d - %.2d - %.2d",rtc_convert_bcd2dec(rtc_date_get_month()),
			rtc_convert_bcd2dec(rtc_date_get_day()),
			rtc_convert_bcd2dec(rtc_date_get_year()));

	printf("Date : %.2d - %.2d - %.2d	",rtc_convert_bcd2dec(rtc_date_get_month()),
			rtc_convert_bcd2dec(rtc_date_get_day()),
			rtc_convert_bcd2dec(rtc_date_get_year()));
}
