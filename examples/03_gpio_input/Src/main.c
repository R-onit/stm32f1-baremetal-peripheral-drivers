#include <stdint.h>
#include "stm32f103xb.h"

#define GPIOAEN		(1U<<2)
#define GPIOCEN		(1U<<4)

#define PIN5		(1U<<5)
#define PIN13		(1U<<13)

#define LED_PIN		PIN5
#define BTN_PIN		PIN13

int main(void){
	RCC->APB2ENR |= GPIOAEN;
	RCC->APB2ENR |= GPIOCEN;

	GPIOA->CRL &= ~(0xF<<(5*4));
	GPIOA->CRL |=  (0x2<<(5*4));

	GPIOC->CRH &= ~(0xF<<(6*4));
	GPIOC->CRH |=  (0x8<<(6*4));


	while(1){


		if(GPIOC->IDR & BTN_PIN){
	    	GPIOA->BSRR =(1UL << (5+16));
		}else{
			GPIOA->BSRR = LED_PIN;

		}
	}
}
