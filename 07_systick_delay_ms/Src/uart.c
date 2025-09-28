#include "uart.h"
#include "stm32f103xb.h"
#include <stdio.h>

#define SYS_CLK    8000000U
#define BAUD_RATE  115200U

void uar2_rxtx_init(void)
{
    /* clocks */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* PA2 TX = AF push-pull: MODE=11 (50MHz), CNF=10 (AF PP) => 0xB */
    GPIOA->CRL &= ~(0xF << (4 * 2));
    GPIOA->CRL |=  (0xB << (4 * 2));

    /* PA3 RX = input floating: MODE=00, CNF=01 => 0x4 */
    GPIOA->CRL &= ~(0xF << (4 * 3));
    GPIOA->CRL |=  (0x4 << (4 * 3));

    set_BaudRate(SYS_CLK, BAUD_RATE);

    /* enable USART2, TE and RE */
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void set_BaudRate(uint32_t clk, uint32_t baud)
{
    /* simple integer round */
    USART2->BRR = (clk + (baud / 2U)) / baud;
}

void USART2_write(char ch)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint16_t)ch;
}

char USART2_read(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)USART2->DR;
}

/* Retarget printf() to USART2: GCC/newlib uses _write() */
int _write(int file, char *ptr, int len)
{
    (void)file;
    for (int i = 0; i < len; ++i) {
        if (ptr[i] == '\n') {
            USART2_write('\r');   // pre-send CR for many terminals
        }
        USART2_write(ptr[i]);
    }
    return len;
}



