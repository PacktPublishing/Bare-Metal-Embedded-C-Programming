#include "iwdg.h"

#define IWDG_KEY_ENABLE                 0x0000CCCCU
#define IWDG_KEY_WR_ACCESS_ENABLE       0x00005555U
#define IWDG_PRESCALER_4				0x00000000U
#define IWDG_RELOAD_VAL					0xFFF

static uint8_t isIwdg_ready(void);

void iwdg_init(void)
{
	/*Enable the IWDG by writing 0x0000CCCC in the IWDG_KR register*/
	IWDG->KR = IWDG_KEY_ENABLE;

	/*Enable register access by writing 0x0000 5555 in the IWDG_KR register*/
	IWDG->KR = IWDG_KEY_WR_ACCESS_ENABLE;

	/*Set the IWDG Prescaler*/
	IWDG->PR =  IWDG_PRESCALER_4;

	/*Set the reload register (IWDG_RLR) to the largest value 0xFFF*/
	IWDG->RLR = IWDG_RELOAD_VAL;

	/*Wait for the registers to be updated (IWDG_SR = 0x0000 0000)*/
	while(isIwdg_ready() != 1){}

	/*Refresh the counter value with IWDG_KR (IWDG_KR = 0x0000 AAAA)*/
	IWDG->KR = IWDG_KEY_RELOAD;
}


static uint8_t isIwdg_ready(void)
{
 return ((READ_BIT(IWDG->SR, IWDG_SR_PVU | IWDG_SR_RVU) == 0U) ? 1UL : 0UL);
}
