#include "exti.h"
#include "stm32f103xb.h"
#define GPIOCEN			(1U<<4)
#define AFIOEN			(1U<<0)


void pc13_exti_init(void)
{
//disable global interrupt
	__disable_irq();
//enable clock access to GPIO
	RCC->APB2ENR |=GPIOCEN;
//enable clock access to AFIO
	RCC->APB2ENR |=AFIOEN;
// Map EXTI13 to port C (PC13 → EXTI13)
	AFIO->EXTICR[3] &= ~(0xF << 4);  // clear bits for EXTI13
	AFIO->EXTICR[3] |=  (0x2 << 4);  // 0x2 = port C
//Unmask EXTI13
	EXTI->IMR |=(1U<<13);
//select falling age trigger
	EXTI->FTSR |=(1U<<13);
//	enable EXTI13 in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);   // unmask in NVIC
//	enable global interrupts
	__enable_irq();
}
