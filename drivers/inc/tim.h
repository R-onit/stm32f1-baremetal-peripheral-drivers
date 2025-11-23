#ifndef TIM_H_
#define TIM_H_

/*******************************Timer2 Delay Driver************************************* */
extern volatile uint32_t tim2_ms_ticks;  
void tim2_1ms_init(void);
void TIM2_IRQHandler(void);
void tim2_delay_ms(uint32_t ms);

/********************************Timer3 PWM Driver************************************** */
void tim3_pwm_init(uint32_t freq_hz);
void tim3_pwm_set_duty(uint8_t percent);

#endif
