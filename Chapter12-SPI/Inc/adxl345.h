

#ifndef ADXL345_H_
#define ADXL345_H_


#include "spi.h"
#include <stdint.h>

#define   ADXL345_REG_DEVID					(0x00)
#define   ADXL345_DEVICE_ADDR    			(0x53)
#define   ADXL345_REG_DATA_FORMAT   		(0x31)
#define   ADXL345_REG_POWER_CTL 			(0x2D)
#define   ADXL345_REG_DATA_START			(0x32)


#define	  ADXL345_RANGE_4G					(0x01)
#define	  ADXL345_RESET						(0x00)
#define   ADXL345_MEASURE_BIT		    	(0x08)

#define   ADXL345_MULTI_BYTE_ENABLE			(0x40)
#define	  ADXL345_READ_OPERATION		    (0x80)


void adxl_init (void);
void adxl_read(uint8_t address, uint8_t * rxdata);

#endif
