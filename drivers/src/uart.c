#include "uart.h"


#define UART2EN		(1U<<17)
#define SYS_CLK		8000000U
#define BAUD_RATE	115200U


void uar2_rxtx_init(void)
{
	/*---------------clock access ----------------*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/*---------------now we need to configure PA2 and PA3---------------------*/
	 // PA2 = TX -> Alternate Function Push-Pull
	GPIOA->CRL &= ~(0xF << (4 * 2));        // Clear MODE2[1:0] and CNF2[1:0]
	GPIOA->CRL |=  (0xB << (4 * 2));        // MODE2 = 11 (50 MHz), CNF2 = 10 (AF Push-Pull)
	// PA3 = RX -> Input Floating
	GPIOA->CRL &= ~(0xF << (4 * 3));        // Clear MODE3[1:0] and CNF3[1:0]
	GPIOA->CRL |=  (0x4 << (4 * 3));        // MODE3 = 00 (input), CNF3 = 01 (floating)

	// 3. Configure Baud Rate for USART2
	set_BaudRate(SYS_CLK,BAUD_RATE);

	// 4. Enable USART2, TX, RX
	USART2->CR1 |= USART_CR1_UE |USART_CR1_TE |USART_CR1_RE;
}

void set_BaudRate(uint32_t clk ,uint32_t baud ){
	USART2->BRR = (clk + (baud/2U))/baud;

}

void USART2_write(char ch){
	 // Wait until TXE (Transmit Data Register Empty) is set
	while(!(USART2->SR & USART_SR_TXE));
	USART2->DR =ch;
}

char USART2_read(){
	 // Wait until RXNE (Recieve Data Register Empty) is set
	while(!(USART2->SR & USART_SR_RXNE));
	return USART2->DR;
}