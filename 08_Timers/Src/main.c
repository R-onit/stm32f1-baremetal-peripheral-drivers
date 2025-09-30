#include <stdio.h>
#include "stm32f103xb.h"
#include "tim.h"

#define SR_UIF				(1U<<0)
int main(void)
{

	//to check the systick we are implementing the led[ PA5 ] blink example
	RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~(0xF<<(5*4));
	GPIOA->CRL |=  (0x2<<(5*4));

	tim2_1hz_init();
	while (1)
    {
		while(!(TIM2->SR & SR_UIF )){}
		TIM2->SR &= ~SR_UIF;
    	GPIOA->ODR ^=(1U<<5);
    }
}
