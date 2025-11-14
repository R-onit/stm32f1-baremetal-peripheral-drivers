#include "stm32f103xb.h"
#include "adc.h"
#include "systick.h"
#include <stdio.h>


//providing clock access to adc1 and gpioa
void adc_init(void)
{
    //providing clock access to adc1 and gpioa
    RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;

    //Configure the PA1 as analog input pin
    GPIOA->CRL &= ~(0xF << (4 * 1));    

    //enabling the ADC1 
    ADC1->CR2 |= ADC_CR2_ADON ;
    //delay 
    systick_delay_ms(100);              // Small stabilization delay

    //lets start calibration
    ADC1->CR2 |= ADC_CR2_CAL;
    //Wait until the calibration is completed
    while (ADC1->CR2 & ADC_CR2_CAL);  

    //deciding the 1 conversion
    ADC1->SQR1 &= ~(0xF << 20);
    //deciding  the channel 
    ADC1->SQR3 =1; 

    // 6. Enable software trigger
    ADC1->CR2 &= ~ADC_CR2_EXTSEL;
    ADC1->CR2 |= ADC_CR2_EXTTRIG;

   
}

    uint32_t singleChannel_singleConversion(void)
    {
        printf("Starting conversion...\r\n");

        //Clearing the EOC 
        ADC1->SR &= ~ADC_SR_EOC;
        ADC1->CR2 |= ADC_CR2_ADON;
        //SOFTWARE STARTING THE ADC
        ADC1->CR2 |= ADC_CR2_SWSTART;
        while(!(ADC1->SR & ADC_SR_EOC)){};
        return ADC1->DR;

    }

// Multi-channel continuous conversion for CH1 (PA1) and CH2 (PA2)
void adc_multichannel_continous_init(void)
{
    // Enable clocks 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;    
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;    

    // Configure PA1 and PA2 as analog input 
    GPIOA->CRL &= ~(0xF << (4 * 1));       // PA1 = analog mode
    GPIOA->CRL &= ~(0xF << (4 * 2));       // PA2 = analog mode

    // Power on ADC 
    ADC1->CR2 |= ADC_CR2_ADON;
    systick_delay_ms(50);                  // small stabilization delay

    // Enable scan mode for multi-channel 
    ADC1->CR1 |= ADC_CR1_SCAN;

    // Start calibration 
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);       // wait for calibration complete

    ADC1->CR2 |= ADC_CR2_CONT;

    // Configure sequence length: L = 1 → 2 conversions 
    ADC1->SQR1 &= ~(0xF << 20);            // clear L bits
    ADC1->SQR1 |=  (1U << 20);             // L = 1 => 2 channels

    // Configure sequence ranks (SQR3) 
    ADC1->SQR3 &= ~(0x1F << 0);            
    ADC1->SQR3 |=  (1U << 0);              

    ADC1->SQR3 &= ~(0x1F << 5);            
    ADC1->SQR3 |=  (2U << 5);              

    // Configure SWSTART trigger 
    ADC1->CR2 &= ~(7 << 17);               
    ADC1->CR2 |=  (7 << 17);               // EXTSEL = 111 (SWSTART)
    ADC1->CR2 |=  ADC_CR2_EXTTRIG;         // enable SWSTART trigger
}

//  Trigger ONE full sequence (CH1→CH2) 
void adc_start_sequence(void)
{
    ADC1->SR &= ~ADC_SR_EOC;               // clear EOC if set
    ADC1->CR2 |= ADC_CR2_SWSTART;          // start sequence
}

// Read ONE conversion result (blocking) 
uint32_t adc_read_next(void)
{
    while (!(ADC1->SR & ADC_SR_EOC));      // wait for conversion
    return ADC1->DR;                       // auto clears EOC
}
