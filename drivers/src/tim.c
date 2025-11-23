#include "stm32f103xb.h"
#include "tim.h"




/*******************************Timer2 Delay Driver******************************* */
void tim2_1ms_init(void)
{
    //enable clock 
    RCC->APB1ENR |=RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 7;  //we would be using 7+1=8 as prescaler , now we have clk=1 MHz (for tim2 periph)
    TIM2->ARR = 999;    //999+1=1000 . so after 1000 clk cycles count would be triggerd 

    TIM2->EGR |= TIM_EGR_UG ;   //reinitialize the PSC,ARR,Counter

    TIM2->DIER |=TIM_DIER_UIE; 
    TIM2->SR &= ~TIM_SR_UIF;

    NVIC_EnableIRQ(TIM2_IRQn);  //enabled NVIC_IRQn

    TIM2->CR1 |= TIM_CR1_CEN; 
}

volatile uint32_t tim2_ms_ticks = 0;
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;    //clear the interrupt flag
        tim2_ms_ticks++;

    }
}

void tim2_delay_ms(uint32_t ms)
{
    uint32_t start =tim2_ms_ticks;
    while((tim2_ms_ticks - start)< ms);
}

/********************************Timer3 PWM Driver************************************** */


void tim3_pwm_init(uint32_t freq_hz)
{
    // enable GPIOA and TIM3 clocks
    RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;

    // configure PA6 as AF PP
    GPIOA->CRL &= ~(0xF << (4*6));
    GPIOA->CRL |=  (0xB << (4*6));


    TIM3->CR1 |= TIM_CR1_ARPE;

    // compute PSC and ARR
    TIM3->PSC = 7;
    TIM3->ARR = (1000000U / freq_hz) - 1;

    // configure PWM mode
    TIM3->CCMR1 &= ~0xFF ;
    TIM3->CCMR1 |= (0x6 << 4);
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;

    // enable channel + timer
    TIM3->CCER &= ~TIM_CCER_CC1P;
    TIM3->CCER |=TIM_CCER_CC1E;// (1U <<1)
    
    // initial duty 
    TIM3->CCR1 = 0;

    TIM3->CR1 |= TIM_CR1_CEN;


}

void tim3_pwm_set_duty(uint8_t percent)
{
    // convert percent → CCR1 based on ARR
    if (percent > 100) percent = 100;

    TIM3->CCR1 = (percent*(TIM3->ARR +1))/100;
}


