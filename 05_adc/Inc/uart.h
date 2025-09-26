#ifndef UART_H_
#define UART_H_
#include <stdint.h>
void uar2_rxtx_init(void);
void set_BaudRate(uint32_t clk, uint32_t baud);
void USART2_write(char ch);
char USART2_read(void);
int _write(int, char*, int);   /* retarget prototype (optional) */
#endif
