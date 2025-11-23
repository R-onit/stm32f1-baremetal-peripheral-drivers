#ifndef UART_H_
#define UART_H_

#include "stm32f103xb.h"
void uart2_rxtx_init(void);
void set_BaudRate(uint32_t clk ,uint32_t baud );
void USART2_write(char ch);
char USART2_read();

extern uint8_t uart2_last_byte;
extern volatile uint8_t uart2_new_data;
void uart2_rxtx_interrupt_init(void);
void USART2_IRQHandler(void);
uint8_t uart2_read_intrr(void);

int uart2_rx_pop(uint8_t *data);


#endif