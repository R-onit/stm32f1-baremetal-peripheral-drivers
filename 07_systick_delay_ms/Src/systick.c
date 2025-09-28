#include "stm32f103xb.h"
#define SYSCLK 8000000U
#define SYSTICK_LOAD_VALUE (SYSCLK/1000 - 1)  // 1 ms tick
#define CTRL_COUNTFLAG				(1U<<16)
#define CTRL_CLKSOURCE_EN			(1U<<2)
#define CTRL_ENABLE					(1U<<0)


void systick_delay_ms(int delay){
//just configure reload value register to 8000000
	SysTick->LOAD =SYSTICK_LOAD_VALUE;
//lets set current value register to 0
	SysTick->VAL =0;
//configure control and status register ,enabling the systick , and selecting between internal and external clk source
	SysTick->CTRL |= CTRL_ENABLE | CTRL_CLKSOURCE_EN ;

	for(int i=0 ; i<delay ; i++){
		while((SysTick->CTRL & CTRL_COUNTFLAG)==0){}
	}
	SysTick->CTRL=0;
}
