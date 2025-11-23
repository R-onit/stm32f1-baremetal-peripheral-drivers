#include "stm32f103xb.h"
#include "systick.h"
#include "tim.h"



int main(void)
{
    tim3_pwm_init();

    while(1)
    {
        for (int d=0 ;d<=100;d+=5)
        {
            tim3_pwm_set_duty(d);
            systick_delay_ms(500);
        }

        for (int d=100 ;d<=0;d-=5)
        {
            tim3_pwm_set_duty(d);
            systick_delay_ms(500);
        }
    }
}