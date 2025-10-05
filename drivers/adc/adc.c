#include "stm32f103xb.h"
#include "adc.h"

/* Configure PA1 as analog input and ADC1 channel1 as single conversion */
void pa1_adc_init(void)
{
    /* 1) enable clocks */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1

    /* 2) PA1 analog mode -> clear bits for pin1 in CRL */
    GPIOA->CRL &= ~(0xF << (1 * 4));      // MODE1 = 00, CNF1 = 00 (analog)

    /* 3) sample time for channel 1 (SMPR2, 3 bits per channel) */
//    ADC1->SMPR2 &= ~(0x7 << (3 * 1));     // clear sample bits for channel1
//    ADC1->SMPR2 |=  (0x7 << (3 * 1));     // set longest sample time (safe for high-Z sources)

    /* 4) sequence: one conversion, channel 1 as rank1 */
    ADC1->SQR1 &= ~(0xF << 20);           // L = 0 => 1 conversion
    ADC1->SQR3 &= ~(0x1F << 0);
    ADC1->SQR3 |=  (1 << 0);              // rank1 = channel 1

    /* 5) enable ADC and run calibration (STM32F1) */
    ADC1->CR2 |= (1U << 0);               // ADON = 1 (power on)
    for (volatile int i = 0; i < 2000; ++i) { __asm("nop"); }  // small delay for stabilization

//    ADC1->CR2 |= (1U << 2);               // CAL = 1
//    while (ADC1->CR2 & (1U << 2));        // wait for CAL to clear
}

/* start a software conversion on regular channels */
void start_conversion(void)
{
//    ADC1->SR &= ~ADC_SR_EOC;              // optional: clear EOC
    ADC1->CR2 |= (1U << 22);              // SWSTART
}

/* blocking read (wait EOC then read DR) */
uint32_t read_conversion(void)
{
    while (!(ADC1->SR & ADC_SR_EOC)) { }  // wait end of conversion
    return (uint32_t)ADC1->DR;            // reading DR clears EOC
}
