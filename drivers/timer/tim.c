#include "stm32f103xb.h"
#include "tim.h"


#define TIM2_EN        (1U<<0)
#define TIM3_EN        (1U<<1)

#define CC2E_EN        (1U<<4)
#define CC1S_EN			(1U<<0)
#define CC1E_EN			(1U<<0)
#define RCC_APB2ENR_IOPA (1U<<2)


void tim2_1hz_init(void){
	//enable clock access to the TIM2
	RCC->APB1ENR |=TIM2_EN;

	//Set prescalar value
	TIM2->PSC = (8000 -1);

	//SET AUTO RELOAD REGISTER
	TIM2->ARR = 1000 - 1;

	//CLEAR COUNTER
	TIM2->CNT =0;

	//ENABLE TIMER
	TIM2->CR1=(1U<<0);
}

void tim2_pa1_output_compare(void){
    // Enable clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPA;
    RCC->APB2ENR |= (1U << 0); // AFIO
    RCC->APB1ENR |= TIM2_EN;

    // Ensure TIM2 is not remapped
    AFIO->MAPR &= ~(1U << 8);

    // PA1 → AF_PP, 2 MHz
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |=  (0xB << 4); // MODE=10 (2 MHz), CNF=10 (AF_PP)

    // Prescaler = 7999 → 1 kHz tick
    TIM2->PSC = 7999;
    // ARR = 999 → 1 Hz overflow
    TIM2->ARR = 999;

    // Toggle on match (CH2)
    TIM2->CCMR1 &= ~(0xF << 12);
    TIM2->CCMR1 |=  (0x3 << 12);

    // Enable CH2 output
    TIM2->CCER |= CC2E_EN;

    // Reset counter
    TIM2->CNT = 0;

    // Enable timer
    TIM2->CR1 |= (1U << 0);
}

void tim3_pa6_input_capture(void){
    // Enable clocks
	RCC->APB2ENR |= RCC_APB2ENR_IOPA;
    // PA6 input , 2 MHz.
	GPIOA->CRL &= ~(0xF << (4*6));
	GPIOA->CRL |=  (0x4 << (4*6));
    // Prescaler = 7999 → 1 kHz tick
	TIM3->PSC = 8000-1;
    // ARR = 999 → 1 Hz overflow
	TIM3->ARR = 1000-1;
	//enable CH1
	TIM3->CCMR1 &= ~(0x3 << 0);   // clear CC1S
	TIM3->CCMR1 |=  (0x1 << 0);   // CC1 channel is mapped on TI1
	//enable CH1 to capture edges
	TIM3->CCER |=CC1E_EN;
	//enable timer
	TIM3->CR1 |=(1U << 0);
}


