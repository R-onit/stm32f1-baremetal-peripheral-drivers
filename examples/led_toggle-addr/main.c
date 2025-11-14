#include <stdint.h>

#define PERIPH_BASE         (0x40000000UL)

/* --- Bus base addresses ---*/
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x10000UL)   // 0x40010000
#define AHBPERIPH_BASE      (PERIPH_BASE + 0x20000UL)   // 0x40020000

/* --- Peripheral base addresses ---*/
#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800UL) // 0x40010800
#define RCC_BASE            (AHBPERIPH_BASE  + 0x1000UL) // 0x40021000

/* --- Registers ---*/
#define RCC_APB2EN_R        (*(volatile uint32_t *)(RCC_BASE   + 0x18UL))
#define GPIOA_CRL           (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))

/* --- Bit positions ---*/
#define GPIOAEN             (1U << 2)
#define LED_PIN             (1U << 5)

int main(void)
{
    // 1. Enable GPIOA clock
    RCC_APB2EN_R |= GPIOAEN;

    // 2. Configure PA5 as output (2MHz push-pull)
    GPIOA_CRL &= ~(0xF << (4*5));   // clear bits for PA5
    GPIOA_CRL |=  (0x2 << (4*5));   //  CNF=00 ,MODE=10,

    // 3. Toggle LED
    while (1)
    {
        GPIOA_ODR ^= LED_PIN;
        for (volatile int i = 0; i < 100000; i++);
    }
}
