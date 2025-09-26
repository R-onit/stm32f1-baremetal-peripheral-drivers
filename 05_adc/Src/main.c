#include <stdio.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"

/* global so debugger can always inspect it; volatile prevents optimization */
volatile uint32_t sensor_value;

int main(void)
{
    /* init peripherals ONCE */
    uar2_rxtx_init();
    pa1_adc_init();

    /* continuous read + print */
    while (1)
    {
        start_conversion();
        sensor_value = read_conversion();               // debugger-friendly global volatile
		printf("sensor_value: %d\r\n", (int)sensor_value);

        /* simple delay so terminal is readable */
        for (volatile int i = 0; i < 300000; ++i) { __asm("nop"); }
    }
}
