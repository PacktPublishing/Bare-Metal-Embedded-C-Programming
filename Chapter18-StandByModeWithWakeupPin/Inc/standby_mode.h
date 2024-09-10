#ifndef STANDBY_MODE_H__
#define STANDBY_MODE_H__

#include <stdint.h>
#include "stm32f4xx.h"

uint32_t get_wakeup_pin_state(void);
void wakeup_pin_init(void);
void standby_wakeup_pin_setup(void);

#endif
