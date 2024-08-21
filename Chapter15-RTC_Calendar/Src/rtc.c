#include "rtc.h"


#define PWREN		(1U<<28)
#define CR_DBP		(1U<<8)
#define CSR_LSION	(1U<<0)
#define CSR_LSIRDY	(1U<<1)
#define BDCR_BDRST	(1U<<16)
#define BDCR_RTCEN	(1U<<15)

#define RTC_WRITE_PROTECTION_KEY_1 ((uint8_t)0xCAU)
#define RTC_WRITE_PROTECTION_KEY_2 ((uint8_t)0x53U)
#define RTC_INIT_MASK                 0xFFFFFFFFU
#define ISR_INITF						(1U<<6)


#define WEEKDAY_FRIDAY             		((uint8_t)0x05U)
#define MONTH_DECEMBER             		((uint8_t)0x12U)
#define TIME_FORMAT_PM             		(1U<<22)
#define CR_FMT							(1U<<6)
#define ISR_RSF							(1U<<5)

#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00F9)

static uint8_t rtc_init_seq(void);
static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year);
static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds);
static void rtc_set_asynch_prescaler(uint32_t AsynchPrescaler);
static void rtc_set_synch_prescaler(uint32_t SynchPrescaler);
static uint8_t exit_init_seq(void);

void rtc_init(void)
{
	/*Enable clock access to PWR */
	RCC->APB1ENR |= PWREN;

	/*Enable Backup access to config RTC*/
	PWR->CR |=CR_DBP;

	/*Enable Low Speed Internal (LSI)*/
	RCC->CSR |=CSR_LSION;

	/*Wait for LSI to be ready*/
	while((RCC->CSR & CSR_LSIRDY) != CSR_LSIRDY){}

	/*Force backup domain reset*/
	RCC->BDCR |=BDCR_BDRST;

	/*Release backup domain reset*/
	RCC->BDCR &= ~BDCR_BDRST;

	/*Set RTC clock source to LSI*/
	RCC->BDCR &=~(1U<<8);
	RCC->BDCR |= (1U<<9);

	/*Enable the RTC*/
	RCC->BDCR |=BDCR_RTCEN;

	/*Disable RTC registers write protection*/
	RTC->WPR = RTC_WRITE_PROTECTION_KEY_1;
	RTC->WPR = RTC_WRITE_PROTECTION_KEY_2;

	/*Enter the initialization mode*/
	if(rtc_init_seq() != 1)
	{
		//Do something...
	}

	/*Set desired date :  Friday December 29th 2016*/
	rtc_date_config(WEEKDAY_FRIDAY,0x29,MONTH_DECEMBER,0x16);

	/*Set desired time :  11:59:55 PM*/
	rtc_time_config(TIME_FORMAT_PM,0x11,0x59,0X55);

	/*Set hour format*/
	RTC->CR |=CR_FMT;

	/*Set Asynch prescaler*/
	rtc_set_asynch_prescaler(RTC_ASYNCH_PREDIV);

	/*Set Sync prescaler*/
	rtc_set_synch_prescaler(RTC_SYNCH_PREDIV);

	/*Exit the initialization mode*/
	exit_init_seq();

	/*Enable RTC registers write protection*/
	RTC->WPR = 0xFF;

}


uint8_t rtc_convert_dec2bcd(uint8_t value)
{
	return  (uint8_t)((((value) / 10U) << 4U) | ((value) % 10U));
}

uint8_t rtc_convert_bcd2dec(uint8_t value )
{
	return (uint8_t)(((uint8_t)((value) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((value) & (uint8_t)0x0FU));
}

uint32_t rtc_date_get_day(void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_DT | RTC_DR_DU))) >> RTC_DR_DU_Pos);
}

uint32_t rtc_date_get_year(void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_YT | RTC_DR_YU))) >> RTC_DR_YU_Pos);
}

uint32_t rtc_date_get_month(void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_MT | RTC_DR_MU)))>> RTC_DR_MU_Pos);
}

uint32_t rtc_time_get_second(void)
{
 return (uint32_t)(READ_BIT(RTC->TR, (RTC_TR_ST | RTC_TR_SU)) >> RTC_TR_SU_Pos);
}

uint32_t rtc_time_get_minute(void)
{
 return (uint32_t)(READ_BIT(RTC->TR, (RTC_TR_MNT | RTC_TR_MNU))>> RTC_TR_MNU_Pos);
}


 uint32_t rtc_time_get_hour(void)
{
  return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_HT | RTC_TR_HU))) >> RTC_TR_HU_Pos);
}

static void rtc_set_asynch_prescaler(uint32_t AsynchPrescaler)
{
  MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, AsynchPrescaler << RTC_PRER_PREDIV_A_Pos);
}



static void rtc_set_synch_prescaler(uint32_t SynchPrescaler)
{
  MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, SynchPrescaler);
}


static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year)
{
  register uint32_t temp = 0U;

  temp = (WeekDay << RTC_DR_WDU_Pos)                                                        | \
         (((Year & 0xF0U) << (RTC_DR_YT_Pos - 4U)) | ((Year & 0x0FU) << RTC_DR_YU_Pos))   | \
         (((Month & 0xF0U) << (RTC_DR_MT_Pos - 4U)) | ((Month & 0x0FU) << RTC_DR_MU_Pos)) | \
         (((Day & 0xF0U) << (RTC_DR_DT_Pos - 4U)) | ((Day & 0x0FU) << RTC_DR_DU_Pos));

  MODIFY_REG(RTC->DR, (RTC_DR_WDU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU | RTC_DR_YT | RTC_DR_YU), temp);
}



static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds)
{
  register uint32_t temp = 0U;

  temp = Format12_24                                                                                    | \
         (((Hours & 0xF0U) << (RTC_TR_HT_Pos - 4U)) | ((Hours & 0x0FU) << RTC_TR_HU_Pos))     | \
         (((Minutes & 0xF0U) << (RTC_TR_MNT_Pos - 4U)) | ((Minutes & 0x0FU) << RTC_TR_MNU_Pos)) | \
         (((Seconds & 0xF0U) << (RTC_TR_ST_Pos - 4U)) | ((Seconds & 0x0FU) << RTC_TR_SU_Pos));
  MODIFY_REG(RTC->TR, (RTC_TR_PM | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU), temp);
}


void _rtc_enable_init_mode(void)
{
	RTC->ISR = RTC_INIT_MASK;
}

void _rtc_disable_init_mode(void)
{
	RTC->ISR = ~RTC_INIT_MASK;
}

uint8_t _rtc_isActiveflag_init(void)
{

	return ((RTC->ISR & ISR_INITF) == ISR_INITF);
}

uint8_t _rtc_isActiveflag_rs(void)
{
	return ((RTC->ISR & ISR_RSF) == ISR_RSF);
}

static uint8_t rtc_init_seq(void)
{
	/*Start init mode*/
	_rtc_enable_init_mode();

	/*Wait till we are in init mode*/
	while(_rtc_isActiveflag_init() != 1){}

	return 1;
}

static uint8_t wait_for_synchro(void)
{
	 /*Clear RSF*/
	RTC->ISR &= ~ISR_RSF;

	/*Wait for registers synch*/
	while(_rtc_isActiveflag_rs() !=1){}

	return 1;
}
static uint8_t exit_init_seq(void)
{
	/*Stop init mode*/
	_rtc_disable_init_mode();

	/*Wait for registers to synchronise*/
	return (wait_for_synchro());
}

