#include "stm32f103xb.h"
#include "tim.h"

#define TIM2_EN			(1U<<0)


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
