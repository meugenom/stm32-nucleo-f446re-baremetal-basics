/**
 * Bare-metal STM32F446RE LED Blink (PA5)
 */

typedef unsigned int uint32_t;

// --- 1. REGISTER ADDRESSES (From RM0390 Reference Manual) ---

// Peripheral clock enable register (RCC AHB1 Enable Register)
#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830U)

// Port A pin mode register (GPIOA Mode Register)
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000U)

// Port A output data register (GPIOA Output Data Register)
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014U)


// Simple delay function
static void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop"); // CPU instruction: "do nothing for 1 cycle"
    }
}

int main(void) {
    // ------------------------------------------------------------------------
    // STEP 1: Enable clock for Port A
    // Bit 0 in RCC_AHB1ENR controls GPIOA. Set it to 1.
    // ------------------------------------------------------------------------
    RCC_AHB1ENR |= (1U << 0);

    // ------------------------------------------------------------------------
    // STEP 2: Configure pin PA5 as Output (General Purpose Output)
    // Each pin in MODER is allocated 2 bits. For PA5, these are bits [11:10].
    // Value 01 = Output.
    // ------------------------------------------------------------------------
    GPIOA_MODER &= ~(3U << (5 * 2)); // Clear bits 11 and 10
    GPIOA_MODER |= (1U << (5 * 2));  // Write 01 to bits 11 and 10

    // ------------------------------------------------------------------------
    // STEP 3: Infinite loop — blink the LED
    // ------------------------------------------------------------------------
    while (1) {
        GPIOA_ODR ^= (1U << 5); // Toggle 5th bit (ON -> OFF -> ON)
        delay(400000);          // Delay
    }
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long* dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();

    for (;;) (void)0;
}

extern void _estack(void);

__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset};