//  1: Define base address for peripherals
#define PERIPH_BASE        (0x40000000UL)
//  2: Offset for AHB1 peripheral bus
#define AHB1PERIPH_OFFSET  (0x00020000UL)
//  3: Base address for AHB1 peripherals
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)
//  4: Offset for GPIOA
#define GPIOA_OFFSET       (0x0000UL)
//  5: Base address for GPIOA
#define GPIOA_BASE         (AHB1PERIPH_BASE + GPIOA_OFFSET)
//  6: Offset for RCC
#define RCC_OFFSET         (0x3800UL)
//  7: Base address for RCC
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)
//  8: Offset for AHB1EN register
#define AHB1EN_R_OFFSET    (0x30UL)
//  9: Address of AHB1EN register
#define RCC_AHB1EN_R  (*(volatile unsigned int *)(RCC_BASE +  AHB1EN_R_OFFSET))
//  10: Offset for mode register
#define MODE_R_OFFSET      (0x00UL)
//  11: Address of GPIOA mode register
#define GPIOA_MODE_R  (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))
//  12: Offset for output data register
#define OD_R_OFFSET   (0x14UL)
//  13: Address of GPIOA output data register
#define GPIOA_OD_R    (*(volatile unsigned int *)(GPIOA_BASE +  OD_R_OFFSET))
//  14: Bit mask for enabling GPIOA (bit 0)
#define GPIOAEN       (1U<<0)
//  15: Bit mask for GPIOA pin 5
#define PIN5          (1U<<5)
//  16: Alias for PIN5 representing LED pin
#define LED_PIN       PIN5

//  17: Start of main function
int main(void)
{
    //  18: Enable clock access to GPIOA
    RCC_AHB1EN_R |= GPIOAEN;

    GPIOA_MODE_R |= (1U<<10);  //  19: Set bit 10 to 1
    GPIOA_MODE_R &= ~(1U<<11); //  20: Set bit 11 to 0

    //  21: Start of infinite loop
    while(1)
    {
    	//  22: Toggle PA5(LED_PIN) 
    	GPIOA_OD_R ^= LED_PIN;
		for(int i = 0; i < 5000000; i++){}

    }  //  23: End of infinite loop

}  //  24: End of main function
