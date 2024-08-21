#include "stm32f4xx.h"



#define 	GPIOBEN			(1U<<1)
#define 	I2C1EN			(1U<<21)

#define 	I2C_100KHZ						80		//0B 0101 0000 = Decimal = 80
#define 	SD_MODE_MAX_RISE_TIME			17
#define		CR1_PE			(1U<<0)

#define		SR2_BUSY		(1U<<1)
#define     CR1_START		(1U<<8)
#define		SR1_SB			(1U<<0)
#define		SR1_ADDR		(1U<<1)
#define		SR1_TXE			(1U<<7)
#define		CR1_ACK			(1U<<10)
#define     CR1_STOP		(1U<<9)
#define		SR1_RXNE		(1U<<6)
#define	    SR1_BTF			(1U<<2)

/*
 * PB8 ---- SCL
 * PB9 ----- SDA
 * */

void i2c1_init(void)
{
	/*Enable clock access to GPIOB*/
	 RCC->AHB1ENR |=GPIOBEN;

	/*Set PB8 and PB9 mode to alternate function*/
	GPIOB->MODER &=~(1U<<16);
	GPIOB->MODER |=(1U<<17);

	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |=(1U<<19);

	/*Set PB8 and PB9 output type to  open drain*/
	GPIOB->OTYPER |=(1U<<8);
	GPIOB->OTYPER |=(1U<<9);

	/*Enable Pull-up for PB8 and PB9*/
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	/*Set PB8 and PB9 alternate function type to I2C (AF4)*/
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |=(1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |=(1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);


	/*Enable clock access to I2C1*/
	 RCC->APB1ENR |= I2C1EN;

	/*Enter reset mode  */
	I2C1->CR1 |= (1U<<15);

	/*Come out of reset mode  */
	I2C1->CR1 &=~(1U<<15);

	/*Set Peripheral clock frequency*/
	I2C1->CR2 = (1U<<4);   //16 Mhz

	/*Set I2C to standard mode, 100kHz clock */
	I2C1->CCR = I2C_100KHZ;

	/*Set rise time */
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/*Enable I2C1 module */
	I2C1->CR1 |= CR1_PE;


}

void i2c1_byte_read(char saddr, char maddr, char* data) {

      volatile int tmp;

	  /* Wait until bus not busy */
	  while (I2C1->SR2 & (SR2_BUSY)){}

	  /* Generate start */
	  I2C1->CR1 |= CR1_START;

	  /* Wait until start flag is set */
	  while (!(I2C1->SR1 & (SR1_SB))){}

	  /* Transmit slave address + Write */
	   I2C1->DR = saddr << 1;

	  /* Wait until addr flag is set */
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	  /* Clear addr flag */
	  tmp = I2C1->SR2;

      /* Send memory address */
      I2C1->DR = maddr;

      /*Wait until transmitter empty */
     while (!(I2C1->SR1 & SR1_TXE)){}

     /*Generate restart */
      I2C1->CR1 |= CR1_START;

      /* Wait until start flag is set */
     while (!(I2C1->SR1 & SR1_SB)){}

     /* Transmit slave address + Read */
     I2C1->DR = saddr << 1 | 1;

     /* Wait until addr flag is set */
    while (!(I2C1->SR1 & (SR1_ADDR))){}

    /* Disable Acknowledge */
    I2C1->CR1 &= ~CR1_ACK;

    /* Clear addr flag */
    tmp = I2C1->SR2;

    /* Generate stop after data received */
     I2C1->CR1 |= CR1_STOP;

    /* Wait until RXNE flag is set */
    while (!(I2C1->SR1 & SR1_RXNE)){}

    /* Read data from DR */
      *data++ = I2C1->DR;
}

void i2c1_burst_read(char saddr, char maddr, int n, char* data) {

	volatile int tmp;

	 /* Wait until bus not busy */
	  while (I2C1->SR2 & (SR2_BUSY)){}

      /* Generate start */
    I2C1->CR1 |= CR1_START;

    /* Wait until start flag is set */
     while (!(I2C1->SR1 & SR1_SB)){}

     /* Transmit slave address + Write */
     I2C1->DR = saddr << 1;

     /* Wait until addr flag is set */
    while (!(I2C1->SR1 & SR1_ADDR)){}

    /* Clear addr flag */
     tmp = I2C1->SR2;

     /* Wait until transmitter empty */
    while (!(I2C1->SR1 & SR1_TXE)){}

    /*Send memory address */
    I2C1->DR = maddr;

    /*Wait until transmitter empty */
    while (!(I2C1->SR1 & SR1_TXE)){}

    /*Generate restart */
    I2C1->CR1 |= CR1_START;

    /* Wait until start flag is set */
    while (!(I2C1->SR1 & SR1_SB)){}

    /* Transmit slave address + Read */
    I2C1->DR = saddr << 1 | 1;

    /* Wait until addr flag is set */
    while (!(I2C1->SR1 & (SR1_ADDR))){}

    /* Clear addr flag */
    tmp = I2C1->SR2;

    /* Enable Acknowledge */
      I2C1->CR1 |=  CR1_ACK;

    while(n > 0U)
    {
    	/*if one byte*/
    	if(n == 1U)
    	{
    		/* Disable Acknowledge */
    	    I2C1->CR1 &= ~CR1_ACK;

    	    /* Generate Stop */
    	    I2C1->CR1 |= CR1_STOP;

 			/* Wait for RXNE flag set */
            while (!(I2C1->SR1 & SR1_RXNE)){}

            /* Read data from DR */
            *data++ = I2C1->DR;
            break;
    	}
    	else
    	{
       	 /* Wait until RXNE flag is set */
           while (!(I2C1->SR1 & SR1_RXNE)){}

           /* Read data from DR */
           (*data++) = I2C1->DR;

           n--;
    	}
    }

}


void i2c1_burst_write(char saddr, char maddr, int n, char* data) {

	volatile int tmp;

	 /* Wait until bus not busy */
	 while (I2C1->SR2 & (SR2_BUSY)){}

     /* Generate start */
    I2C1->CR1 |= CR1_START;

    /* Wait until start flag is set */
    while (!(I2C1->SR1 & (SR1_SB))){}

    /* Transmit slave address */
    I2C1->DR = saddr << 1;

    /* Wait until addr flag is set */
    while (!(I2C1->SR1 & (SR1_ADDR))){}

    /* Clear addr flag */
    tmp = I2C1->SR2;

    /* Wait until data register empty */
    while (!(I2C1->SR1 & (SR1_TXE))){}

    /* Send memory address */
    I2C1->DR = maddr;

    for (int i = 0; i < n; i++) {

     /* Wait until data register empty */
        while (!(I2C1->SR1 & (SR1_TXE))){}

      /* Transmit memory address */
      I2C1->DR = *data++;
    }

    /* Wait until transfer finished */
    while (!(I2C1->SR1 & (SR1_BTF))){}

    /* Generate stop */
   I2C1->CR1 |= CR1_STOP;
}





