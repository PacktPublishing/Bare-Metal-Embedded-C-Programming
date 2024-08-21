#include "uart_dma.h"

#define UART2EN			(1U<<17)
#define GPIOAEN			(1U<<0)

#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)

#define CR3_DMAT		(1U<<7)
#define CR3_DMAR		(1U<<6)
#define SR_TC			(1U<<6)
#define CR1_TCIE		(1U<<6)

#define UART_BAUDRATE	115200
#define CLK				16000000

#define DMA1EN			    (1U<<21)
#define DMA_SCR_EN  		(1U<<0)
#define DMA_SCR_MINC		(1U<<10)
#define DMA_SCR_PINC		(1U<<9)
#define DMA_SCR_CIRC		(1U<<8)
#define DMA_SCR_TCIE		(1U<<4)
#define DMA_SCR_TEIE		(1U<<2)
#define DMA_SFCR_DMDIS		(1U<<2)

#define HIFCR_CDMEIF5		(1U<<8)
#define HIFCR_CTEIF5		(1U<<9)
#define HIFCR_CTCIF5		(1U<<11)

#define HIFCR_CDMEIF6		(1U<<18)
#define HIFCR_CTEIF6		(1U<<19)
#define HIFCR_CTCIF6		(1U<<21)

#define HIFSR_TCIF5		(1U<<11)
#define HIFSR_TCIF6		(1U<<21)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

char uart_data_buffer[UART_DATA_BUFF_SIZE];

uint8_t g_rx_cmplt;
uint8_t g_tx_cmplt;

uint8_t g_uart_cmplt;

void uart2_rx_tx_init(void)
{
	/*************Configure UART GPIO pin********************/
	/*1.Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*2.Set PA2 mode to alternate function mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |=	 (1U<<5);

	/*3.Set PA3 mode to alternate function mode*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |=	 (1U<<7);

	/*4.Set PA2 alternate function function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/*5.Set PA3 alternate function function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);
	/*************Configure UART Module********************/

	/*6. Enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*7. Set baudrate*/
	uart_set_baudrate(CLK,UART_BAUDRATE);

	/*8. Select to use DMA for TX and RX*/
	USART2->CR3 = CR3_DMAT |CR3_DMAR;

	/*9. Set transfer direction*/
	 USART2->CR1 = CR1_TE |CR1_RE;

	/*10.Clear TC flag*/
	 USART2->SR &=~SR_TC;

	/*11.Enable TCIE*/
	 USART2->CR1 |=CR1_TCIE;

	/*12. Enable uart module*/
	 USART2->CR1 |= CR1_UE;

	 /*13.Enable USART2 interrupt in the NVIC*/
	 NVIC_EnableIRQ(USART2_IRQn);

}


void dma1_init(void)
{
   /*Enable clock access to DMA*/
	RCC->AHB1ENR |=DMA1EN;

	/*Enable DMA Stream6 Interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}


void dma1_stream5_uart_rx_config(void)
{
	/*Disable DMA stream*/
	DMA1_Stream5->CR &=~DMA_SCR_EN;

	/*Wait till DMA Stream is disabled*/
	while((DMA1_Stream5->CR & DMA_SCR_EN)){}

	/*Clear interrupt flags for stream 5*/
	DMA1->HIFCR = HIFCR_CDMEIF5 |HIFCR_CTEIF5|HIFCR_CTCIF5;
	/*Set periph address*/
	DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

	/*Set mem address*/
	DMA1_Stream5->M0AR = (uint32_t)(&uart_data_buffer);

	/*Set number of transfer*/
	DMA1_Stream5->NDTR = (uint16_t)UART_DATA_BUFF_SIZE;

	/*Select Channel 4*/
	DMA1_Stream5->CR &= ~(1u<<25);
	DMA1_Stream5->CR &= ~(1u<<26);
	DMA1_Stream5->CR |= (1u<<27);

	/*Enable memory addr increment*/
	DMA1_Stream5->CR |=DMA_SCR_MINC;

	/*Enable transfer complete interrupt*/
	DMA1_Stream5->CR |= DMA_SCR_TCIE;

	/*Enable Circular mode*/
	DMA1_Stream5->CR |=DMA_SCR_CIRC;

	/*Set transfer direction : Periph to Mem*/
	DMA1_Stream5->CR &=~(1U<<6);
	DMA1_Stream5->CR &=~(1U<<7);

	/*Enable DMA stream*/
	DMA1_Stream5->CR |= DMA_SCR_EN;

	/*Enable DMA Stream5 Interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

void dma1_stream6_uart_tx_config(uint32_t msg_to_snd, uint32_t msg_len)
{
	/*Disable DMA stream*/
	DMA1_Stream6->CR &=~DMA_SCR_EN;

	/*Wait till  DMA Stream is disabled*/
	while((DMA1_Stream6->CR & DMA_SCR_EN)){}

	/*Clear interrupt flags for stream 6*/
	DMA1->HIFCR = HIFCR_CDMEIF6 |HIFCR_CTEIF6|HIFCR_CTCIF6;

	/*Set periph address*/
	DMA1_Stream6->PAR = (uint32_t)(&(USART2->DR));

	/*Set mem address*/
	DMA1_Stream6->M0AR = msg_to_snd;

	/*Set number of transfer*/
	DMA1_Stream6->NDTR = msg_len;

	/*Select Channel 4*/
	DMA1_Stream6->CR &= ~(1u<<25);
	DMA1_Stream6->CR &= ~(1u<<26);
	DMA1_Stream6->CR |= (1u<<27);

	/*Enable memory addr increment*/
	DMA1_Stream6->CR |=DMA_SCR_MINC;

	/*Set transfer direction :Mem to Periph*/
	DMA1_Stream6->CR |=(1U<<6);
	DMA1_Stream6->CR &=~(1U<<7);

	/*Set transfer complete interrupt*/
	DMA1_Stream6->CR |= DMA_SCR_TCIE;

	/*Enable DMA stream*/
	DMA1_Stream6->CR |= DMA_SCR_EN;

}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk +( baudrate/2U ))/baudrate);
}


static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR  = compute_uart_bd(periph_clk,baudrate);
}


void DMA1_Stream6_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF6)
	{
		//do_ssomething
		g_tx_cmplt = 1;
		/*Clear the flag*/
		DMA1->HIFCR |= HIFCR_CTCIF6;
	}
}


void DMA1_Stream5_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF5)
	{

		g_rx_cmplt = 1;


		/*Clear the flag*/
		DMA1->HIFCR |= HIFCR_CTCIF5;
	}
}
void USART2_IRQHandler(void)
{
	g_uart_cmplt  = 1;

	/*Clear TC interrupt flag*/
	USART2->SR &=~SR_TC;
}


