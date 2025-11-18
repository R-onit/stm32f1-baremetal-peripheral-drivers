#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

void adc_init(void);
uint32_t singleChannel_singleConversion(void);

void adc_multichannel_continous_init(void);
void adc_start_sequence(void);
uint32_t adc_read_next(void);


#endif
