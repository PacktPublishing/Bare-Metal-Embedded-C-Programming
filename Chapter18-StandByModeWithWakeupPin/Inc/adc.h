#ifndef ADC_H__
#define ADC_H__

#include <stdint.h>
#include "stm32f4xx.h"

void pa1_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif

