#include "adxl345.h"

 /********PINOUT********/
/* STM32 ---- ADXL345*/
/*
 * PB8   ----  SCL
 * PB9   ----  SDA
 * GND   ----  GND
 * 5V+   ----  VCC
 * */


// Variable to store single byte of data
char data;

// Buffer to store multiple bytes of data from the ADXL345
uint8_t data_buffer[6];

void adxl_read_address (uint8_t reg)
{
	 i2c1_byte_read( ADXL345_DEVICE_ADDR, reg, &data);

}

void adxl_write (uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	i2c1_burst_write( ADXL345_DEVICE_ADDR, reg,1, data) ;
}

void adxl_read_values (uint8_t reg)
{
    // Read 6 bytes into wthe data buffer

	i2c1_burst_read(ADXL345_DEVICE_ADDR, reg, 6,(char *)data_buffer);

}


void adxl_init (void)
{
	/*Enable I2C*/
	i2c1_init();

	/*Read the DEVID, this should return 0xE5*/
	adxl_read_address(ADXL345_REG_DEVID);

	/*Set data format range to +-4g*/
	adxl_write (ADXL345_REG_DATA_FORMAT, ADXL345_RANGE_4G);

	/*Reset all bits*/
	adxl_write (ADXL345_REG_POWER_CTL, ADXL345_RESET);

	/*Configure power control measure bit*/
	adxl_write (ADXL345_REG_POWER_CTL, ADXL345_MEASURE_BIT);
}
