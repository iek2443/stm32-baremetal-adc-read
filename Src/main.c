#include <stdint.h>

// ==============================
// Base Address Definitions
// ==============================

#define PERIPH_BASE             (0x40000000UL)                      // Base address for all peripherals
#define AHB1PERIPH_OFFSET       (0x00020000UL)
#define AHB1PERIPH_BASE         (PERIPH_BASE + AHB1PERIPH_OFFSET)

#define APB2PERIPH_OFFSET       (0x00010000UL)
#define APB2PERIPH_BASE         (PERIPH_BASE + APB2PERIPH_OFFSET)

#define GPIOA_OFFSET            (0x0000UL)
#define RCC_OFFSET              (0x3800UL)
#define ADC1_OFFSET             (0x2000UL)

#define GPIOA_BASE              (AHB1PERIPH_BASE + GPIOA_OFFSET)
#define RCC_BASE                (AHB1PERIPH_BASE + RCC_OFFSET)
#define ADC1_BASE               (APB2PERIPH_BASE + ADC1_OFFSET)

// Clock enable bits
#define GPIOAEN                 (1U << 0)                           // Enable clock to GPIOA
#define ADC1EN                  (1U << 8)                           // Enable clock to ADC1

// ADC configuration bits
#define ADC_CH1                 (1U << 0)                           // ADC channel 1 (PA1)
#define ADC_SEQ_LEN             0x00                                // Sequence length = 1
#define CR2_ADCON               (1U << 0)                           // ADC enable
#define CR2_SWSTART             (1U << 30)                          // Start conversion
#define SR_EOC                  (1U << 1)                           // End of conversion flag

// ==============================
// Typedef Struct Definitions
// ==============================

#define __IO volatile

// GPIO register structure
typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
} GPIO_TypeDef;

// RCC register structure
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t AHB3ENR;
    uint32_t RESERVED2;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    __IO uint32_t AHB1LPENR;
    __IO uint32_t AHB2LPENR;
    __IO uint32_t AHB3LPENR;
    uint32_t RESERVED4;
    __IO uint32_t APB1LPENR;
    __IO uint32_t APB2LPENR;
    uint32_t RESERVED5[2];
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    uint32_t RESERVED6[2];
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
    __IO uint32_t PLLSAICFGR;
    __IO uint32_t DCKCFGR;
} RCC_TypeDef;

// ADC register structure
typedef struct {
    __IO uint32_t SR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    __IO uint32_t JOFR1;
    __IO uint32_t JOFR2;
    __IO uint32_t JOFR3;
    __IO uint32_t JOFR4;
    __IO uint32_t HTR;
    __IO uint32_t LTR;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t JSQR;
    __IO uint32_t JDR1;
    __IO uint32_t JDR2;
    __IO uint32_t JDR3;
    __IO uint32_t JDR4;
    __IO uint32_t DR;
    __IO uint32_t CSR;
    __IO uint32_t CCR;
    __IO uint32_t CDR;
} ADC_TypeDef;

// Peripheral base pointers
#define RCC     ((RCC_TypeDef *) RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *) GPIOB_BASE)
#define USART2  ((USART_TypeDef *) USART2_BASE)
#define USART3  ((USART_TypeDef *) USART3_BASE)
#define ADC1    ((ADC_TypeDef *) ADC1_BASE)

// ==============================
// Function Prototypes
// ==============================
void pa1_adc_init(void);         // Initialize PA1 as analog input
void start_converstion(void);    // Start ADC conversion
uint32_t adc_read(void);         // Read ADC value

// ==============================
// Main Function
// ==============================

uint32_t adc_value;

int main(void) {

    pa1_adc_init();              // Configure ADC1 to read from PA1 (channel 1)

    while (1) {
        start_converstion();     // Trigger a new ADC conversion

        adc_value = adc_read();  // Wait and read ADC result

        for (int i = 0; i < 100000; i++) {
            __asm__("nop");      // Simple delay
        }
    }
}

// ==============================
// ADC Initialization
// ==============================

void pa1_adc_init(void){
    /* Enable clock access to GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Set PA1 to analog mode */
    GPIOA->MODER |= (1U << 2);
    GPIOA->MODER |= (1U << 3);

    /********** Configure ADC module **********/

    /* Enable clock access to ADC1 */
    RCC->APB2ENR |= ADC1EN;

    /* Set ADC conversion channel to channel 1 (PA1) */
    ADC1->SQR3 = ADC_CH1;

    /* Set conversion sequence length = 1 */
    ADC1->SQR1 = ADC_SEQ_LEN;

    /* Enable the ADC module */
    ADC1->CR2 |= CR2_ADCON;
}

void start_converstion(void){
    /* Start ADC conversion */
    ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){
    /* Wait until conversion is complete (EOC flag) */
    while (!(ADC1->SR & SR_EOC)){
        __asm__("nop"); // prevent optimization
    }

    /* Return the converted result from data register */
    return (ADC1->DR);
}
