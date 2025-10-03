#include <stdio.h>
#include "stm32f103xb.h"
#include "tim.h"

#define SR_UIF				(1U<<0)
#define SR_CC1IF			(1U<<1)

int timestamp=0;
int main(void)
{

	tim2_pa1_output_compare();
	tim3_pa6_input_capture();
	while(1){

		//wait until edge gets captured
		while(!(TIM3->SR & SR_CC1IF)){}

		timestamp=TIM3->CCR1;

}
}
