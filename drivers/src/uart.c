#include "uart.h"


#define UART2EN		(1U<<17)
#define SYS_CLK		8000000U
#define BAUD_RATE	115200U


uint8_t uart2_last_byte = 0;
volatile uint8_t uart2_new_data = 0;

#define UART2_RX_BUF_SIZE 16
#define UART2_RX_BUF_MASK (UART2_RX_BUF_SIZE - 1)

volatile uint8_t uart2_rx_buffer[UART2_RX_BUF_SIZE];
volatile uint8_t uart2_rx_head = 0;
volatile uint8_t uart2_rx_tail = 0;

static inline void uart2_rx_push(uint8_t data)
{
	uint8_t next = (uart2_rx_head + 1) & UART2_RX_BUF_MASK;
	if(next == uart2_rx_tail){
		// condition where tail equals to head , so  buffer is buff no place 
		return;
	}
	uart2_rx_buffer[uart2_rx_head]=data;
	uart2_rx_head= next;

}

int uart2_rx_pop(uint8_t *data)
{
	if(uart2_rx_head == uart2_rx_tail){
		// empty
		return 0;
	}
	*data =uart2_rx_buffer[uart2_rx_tail];
	uart2_rx_tail = (uart2_rx_tail +1) & UART2_RX_BUF_MASK;
	return 1;
}

void USART2_IRQHandler(void)
{
	if(USART2->SR & USART_SR_RXNE)
	{
		uint8_t b = USART2->DR;
		uart2_rx_push(b);

	}
}




void uart2_rxtx_init(void)
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




void uart2_rxtx_interrupt_init(void)
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

	//  Configure Baud Rate for USART2
	set_BaudRate(SYS_CLK,BAUD_RATE);

	//  Enable USART2, TX, RX
	USART2->CR1 |= USART_CR1_UE |USART_CR1_TE |USART_CR1_RE;

	// enabling interrupts 
	USART2->CR1 |= USART_CR1_RXNEIE;

	NVIC_EnableIRQ(USART2_IRQn);
}

// void USART2_IRQHandler(void)
// {
// 	uart2_last_byte=USART2->DR;
// 	uart2_new_data=1;
// }


uint8_t uart2_read_intrr(void)
{
	uart2_new_data=0;
	return uart2_last_byte;
}


