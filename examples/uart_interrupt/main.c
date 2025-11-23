#include "stm32f103xb.h"
#include "uart.h"



int main (void)
{
    uart2_rxtx_interrupt_init();

    while(1)
    {
        if(uart2_new_data)
        {
            uint8_t data = uart2_read_intrr();
            USART2_write(data);
        }
    }

}