
// 0: Include standard integer types header for fixed-width integer types
#include <stdint.h>

// 1: GPIO_TypeDef structure definition
typedef struct
{
  volatile uint32_t MODER;    /*offset: 0x00      */
  volatile uint32_t OTYPER;   /*offset: 0x04      */
  volatile uint32_t OSPEEDR;  /*offset: 0x08      */
  volatile uint32_t PUPDR;    /*offset: 0x0C      */
  volatile uint32_t IDR;      /*offset: 0x10      */
  volatile uint32_t ODR;      /*offset: 0x14      */
  volatile uint32_t BSRR;     /*offset: 0x18      */
  volatile uint32_t LCKR;     /*offset: 0x1C      */
  volatile uint32_t AFRL;     /*offset: 0x20	 */
  volatile uint32_t AFRH;     /*offset: 0x24 	 */

} GPIO_TypeDef;

// 2: RCC_TypeDef structure definition
typedef struct
{
  volatile uint32_t DUMMY[12];
  volatile uint32_t AHB1ENR;       /*offset: 0x30*/

} RCC_TypeDef;


// 3: Base address definitions
#define 	RCC_BASE	 0x40023800
#define 	GPIOA_BASE   0x40020000

// 4: Peripheral pointer definitions
#define RCC		    ((RCC_TypeDef*) RCC_BASE)
#define GPIOA		((GPIO_TypeDef*)GPIOA_BASE)

//5: Bit mask for enabling GPIOA (bit 0)
#define GPIOAEN       (1U<<0)
//6: Bit mask for GPIOA pin 5
#define PIN5          (1U<<5)
//7: Alias for PIN5 representing LED pin
#define LED_PIN       PIN5

//  8: Start of main function
int main(void)
{
    //  9: Enable clock access to GPIOA
	 RCC->AHB1ENR |=  GPIOAEN;

	 GPIOA->MODER |= (1U<<10);  //  10: Set bit 10 to 1
	 GPIOA->MODER &= ~(1U<<11); //  11: Set bit 11 to 0

    //  21: Start of infinite loop
    while(1)
    {
    	//  12: Set PA5(LED_PIN) high
		GPIOA->ODR^= LED_PIN;

        // 13: Simple delay
		for(int i=0;i<100000;i++){}

    }

}
