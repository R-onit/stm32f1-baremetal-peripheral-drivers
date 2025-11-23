#include "stm32f103xb.h"
#include "uart.h"


int main (void)
{
    uart2_rxtx_interrupt_init();
    uint8_t b;

    while(1)
    {
        if(uart2_rx_pop(&b))
        {
            USART2_write(b);
        }
    }
}