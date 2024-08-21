#ifndef __IWDG_H__
#define __IWDG_H__

#include <stdint.h>
#include "stm32f4xx.h"


#define IWDG_KEY_RELOAD                 0x0000AAAAU


void iwdg_init(void);

#endif
