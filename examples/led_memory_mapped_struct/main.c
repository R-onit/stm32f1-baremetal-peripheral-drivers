#include <stdint.h>

#define PERIPH_BASE         (0x40000000UL)

/* --- Bus base addresses ---*/
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x10000UL)   // 0x40010000
#define AHBPERIPH_BASE      (PERIPH_BASE + 0x20000UL)   // 0x40020000

/* --- Peripheral base addresses ---*/
#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800UL) // 0x40010800
#define RCC_BASE            (AHBPERIPH_BASE  + 0x1000UL) // 0x40021000

/* --- Bit positions ---*/
#define GPIOAEN             (1U << 2)
#define LED_PIN             (1U << 5)

#define RCC 				((RCC_TypeDef *)RCC_BASE)
#define GPIOA				((GPIOA_TypeDef *)GPIOA_BASE)

typedef struct
{
  volatile uint32_t DUMMY[5];
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
} RCC_TypeDef;

typedef struct
{
  volatile uint32_t CRL;
  volatile uint32_t DUMMY[2];
  volatile uint32_t ODR;
} GPIOA_TypeDef;

int main(void)
{
    // 1. Enable GPIOA clock
	RCC->APB2ENR |=GPIOAEN;

    // 2. Configure PA5 as output (2MHz push-pull)
	GPIOA->CRL &= ~(0xF << (4*5));		// clear bits for PA5
	GPIOA->CRL |= (0x2 << (4*5)); 		//  CNF=00 ,MODE=10,
    // 3. Toggle LED
    while (1)
    {
    	GPIOA->ODR ^= LED_PIN;
    	for (volatile int i = 0; i < 100000; i++);
    }
}

