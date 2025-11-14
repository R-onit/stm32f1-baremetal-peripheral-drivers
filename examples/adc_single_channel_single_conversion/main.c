#include <stdio.h>
#include "stm32f103xb.h"
#include "uart.h"   
#include "adc.h"
#include "systick.h"


extern int _write(int, char*, int);

int main(void)
{
    uart2_rxtx_init();   

    adc_init();


    uint32_t adc_value = 0;
    while (1)
    {
        uint32_t adc_value = singleChannel_singleConversion();
        int millivolts = (adc_value * 3300) / 4095;
        printf("Voltage: %d mV\r\n", millivolts);
        systick_delay_ms(500);
    }
    
}

