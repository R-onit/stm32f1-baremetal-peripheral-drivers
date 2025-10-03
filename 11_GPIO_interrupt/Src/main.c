#include <stdio.h>
#include "stm32f103xb.h"
#include "exti.h"

void gpio_init(void);


int main(void)
{
	pc13_exti_init();
	//PA5


	while(1){

	}
}


void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & LINE13)!=0){

		//clear PR again
		EXTI->PR |=LINE13;
		//do something
		gpio_init();

	}
	//PR
}

