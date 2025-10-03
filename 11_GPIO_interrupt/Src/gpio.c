#include "stm32f103xb.h"
#define GPIOAEN				(1U<<2)
#define LED_PIN				(1U<<5)
void gpio_init(void)
{
	//enable clock access to GPIOC
	RCC->APB2ENR |=GPIOAEN;
	//PORT CONFIGUREATION
	GPIOA->CRL &= ~(0xF << (5*4));
	GPIOA->CRL |= (0x2 << (5*4));
	//TOGGLING PIN
	GPIOA->ODR ^= LED_PIN;
	for (volatile int i = 0; i < 100000; i++);
}



//PA5
