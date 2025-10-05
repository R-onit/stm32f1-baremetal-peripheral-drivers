#include <stdio.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"
#include "systic.h"

int main(void)
{
	//to check the systick we are implementing the led[ PA5 ] blink example
	RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~(0xF<<(5*4));
	GPIOA->CRL |=  (0x2<<(5*4));
//0010
    while (1)
    {
    	GPIOA->ODR ^=(1U<<5);
    	systick_delay_ms(1000);

    }
}
