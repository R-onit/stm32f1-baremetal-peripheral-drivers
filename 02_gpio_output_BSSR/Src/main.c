#include <stdint.h>
#include "stm32f103xb.h"

#define GPIOAEN		(1U<<2)
#define LED_PIN		(1U<<5)
int main(void){
	RCC->APB2ENR |= GPIOAEN;

	GPIOA->CRL &= ~(0xFF<<(5*4)); //CLEARNING ALL THE BITS
	GPIOA->CRL |=  (0x2<<(5*4));

	while(1){
// BSRR---bit set reset register (operates in write only mode , set or reset a single pin atomically. )
// BRR---for clearing bits only
		GPIOA->BSRR = (1UL << 5);
    	for (volatile int i = 0; i < 100000; i++);
    	GPIOA->BSRR =(1UL << (5+16));
    	for (volatile int i = 0; i < 100000; i++);

	}
}
