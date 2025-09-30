#include <stdio.h>
#include "stm32f103xb.h"
#include "tim.h"

#define SR_UIF				(1U<<0)
int main(void)
{

	tim2_pa1_output_compare();
	while (1)
    {

    }
}
