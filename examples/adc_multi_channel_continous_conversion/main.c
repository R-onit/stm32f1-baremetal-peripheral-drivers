#include <stdio.h>
#include "stm32f103xb.h"
#include "uart.h"   
#include "adc.h"
#include "systick.h"


extern int _write(int, char*, int);

int main(void)
{
    uart2_rxtx_init();   
    adc_multichannel_continous_init();
    printf("init ran");
    
    while (1)
    {
        adc_start_sequence();
        printf("start sequence ran ");
        uint32_t ch1 = adc_read_next();   // first value (CH1)
        uint32_t ch2 = adc_read_next();   // second value (CH2)
        printf("start readnext ran ");

        int mv1 = (ch1 * 3300) / 4095;
        int mv2 = (ch2 * 3300) / 4095;
        
        printf("CH1: %d mV   CH2: %d mV\r\n", mv1, mv2);

        systick_delay_ms(500);
    }
    
}

