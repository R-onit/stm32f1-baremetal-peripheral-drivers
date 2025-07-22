#define STM32F103xB
#include "stm32f1xx.h"
#include <stdint.h>


#define PIN5       (1U << 5)
#define SYS_CLK    16000000U
#define BAUD_RATE  115200U

void uart2_tx_init(void);
static void set_baudRate(uint32_t clk, uint32_t baud);
void USART2_write(char ch);
char USART2_read(void);
void delay(volatile uint32_t count);

int main(void) {
    uart2_tx_init();  // Initialize UART2

    while (1) {
        // Read a character from serial terminal and echo it back
        char received = USART2_read();
        USART2_write(received);
    }
}

void uart2_tx_init(void) {
    // 1. Enable Clocks for GPIOA, AFIO and USART2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;     // Enable AFIO for alternate functions
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   // Enable USART2 clock

    // 2. Configure GPIOA Pins
    // PA2 = TX -> Alternate Function Push-Pull
    GPIOA->CRL &= ~(0xF << (4 * 2));        // Clear MODE2[1:0] and CNF2[1:0]
    GPIOA->CRL |=  (0xB << (4 * 2));        // MODE2 = 11 (50 MHz), CNF2 = 10 (AF Push-Pull)

    // PA3 = RX -> Input Floating
    GPIOA->CRL &= ~(0xF << (4 * 3));        // Clear MODE3[1:0] and CNF3[1:0]
    GPIOA->CRL |=  (0x4 << (4 * 3));        // MODE3 = 00 (input), CNF3 = 01 (floating)

    // 3. Configure Baud Rate for USART2
    set_baudRate(SYS_CLK, BAUD_RATE);

    // 4. Enable USART2, TX, RX
    USART2->CR1 |= USART_CR1_TE  | USART_CR1_RE | USART_CR1_UE;
}

static void set_baudRate(uint32_t clk, uint32_t baud) {
    // Simple baud rate calculation assuming APB1 clock = system clock
    USART2->BRR = (clk + (baud / 2U)) / baud;
}

void USART2_write(char ch) {
    // Wait until TXE (Transmit Data Register Empty) is set
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = ch;
}

char USART2_read(void) {
    // Wait until RXNE (Read Data Register Not Empty) is set
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}

void delay(volatile uint32_t count) {
    while (count--);  // crude delay loop
}













