#include "stm32f103xb.h"
#include "tim.h"
#include <stdint.h>



int main (void){

    //enabling clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    //configuring pins
    GPIOA->CRL &= ~(0xF << (5*4));
    GPIOA->CRL |= (0x2 << (5*4));
    //timer initialzation
    tim2_1ms_init();
    while(1)
    {
        uint32_t last_toggle;
        if ((tim2_ms_ticks - last_toggle) >= 500)
        {
            GPIOA->ODR ^= (1<<5);
            last_toggle =tim2_ms_ticks;
        }
    }
}