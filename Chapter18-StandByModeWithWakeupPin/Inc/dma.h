#ifndef __DMA_H__
#define __DMA_H__
#include <stdint.h>
#include "stm32f4xx.h"

#define LISR_TCIF0		    (1U<<5)
#define LIFCR_CTCIF0		(1U<<5)

#define LISR_TEIF0			(1U<<3)
#define LIFCR_CTEIF0		(1U<<3)

void dma2_mem2mem_config(void);
void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);

#endif
