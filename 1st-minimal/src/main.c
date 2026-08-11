/**
 * Minimal Bare-Metal STM32 Application
 * Demonstrates basic step counting (1, 2, 3...) and sum accumulation
 * (1, 3, 6, 10, 15...) suitable for GDB / Watch Expressions during debugging.
 */

typedef unsigned int uint32_t;

int main(void) {
    // 'volatile' forces compiler to keep variables in SRAM without optimizing them away
    volatile uint32_t count = 0; // Increments step by step: 1, 2, 3, 4, ...
    volatile uint32_t sum = 0;   // Cumulative sum: 1, 3, 6, 10, 15, 21, ...

    while (1) {
        count++;      // Step counter (+1 each iteration)
        sum += count; // Accumulate running sum (e.g., 1, 1+2=3, 3+3=6, 6+4=10...)

        // Explicitly cast to void to inform the compiler these values are intentionally used
        (void)count;
        (void)sum;
    }
}

/**
 * Reset Handler Function (_reset)
 * * Executed hardware-automatically on power-up / reset before main():
 * 1. Zeroes out the .bss section in SRAM.
 * 2. Copies initial values of global variables from Flash to SRAM (.data).
 */
__attribute__((naked, noreturn)) void _reset(void) {
    // Memory boundary markers defined in Linker Script (f446.ld)
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    // 1. Clear uninitialized global variables (.bss) in SRAM
    for (long* dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }

    // 2. Copy initialized global variable values from FLASH (_sidata) to SRAM (_sdata)
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) {
        *dst++ = *src++;
    }

    // Transfer control to main application
    main();

    // Fallback infinite safety loop in case main() ever returns
    for (;;) {
        (void)0;
    }
}

// Initial Main Stack Pointer (MSP) symbol defined in linker script
extern void _estack(void);

/**
 * ARM Cortex-M Vector Table (.vectors section)
 * Placed at the very start of Flash (0x08000000).
 * Word 0: Initial Stack Pointer address (_estack)
 * Word 1: Address of Reset Handler (_reset)
 */
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack,  // Vector 0: Stack Top Address
    _reset    // Vector 1: Reset Vector / Entry Point
};