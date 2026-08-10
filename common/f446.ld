/* Specifies to the debugger (GDB) which function is the program entry point */
ENTRY(_reset);

/* Block describing the physical memory regions of the microcontroller */
MEMORY {
  /* * flash(rx) - Non-volatile Flash memory:
   * r = read (readable)
   * x = execute (machine code can be executed from here)
   * ORIGIN = 0x08000000 - base physical address of Flash in STM32
   * LENGTH = 512k       - size of Flash memory (512 Kilobytes)
   */
  flash(rx) : ORIGIN = 0x08000000, LENGTH = 512k

  /* * sram(rwx) - Random Access Memory (SRAM):
   * r = read (readable)
   * w = write (writable for variables at runtime)
   * x = execute (code execution from RAM if required)
   * ORIGIN = 0x20000000 - base physical address of SRAM in STM32
   * LENGTH = 128k       - size of RAM (128 Kilobytes)
   */
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 128k
}

/* * Top of stack calculation:
 * In ARM Cortex-M architecture, the stack grows downwards (from higher to lower addresses).
 * 0x20000000 (start) + 128 KB (size) = 0x20020000 (very end of SRAM).
 * Symbol _estack will hold this address and will be written to address 0x08000000.
 */
_estack = ORIGIN(sram) + LENGTH(sram);

/* Section allocation block across memory regions */
SECTIONS {

  /* 1. Interrupt vector table section */
  .vectors : { 
    KEEP(*(.vectors))   /* KEEP prevents the linker from discarding this section during GC */
  } > flash             /* Placed strictly at the beginning of FLASH (at address 0x08000000) */

  /* 2. Executable code section */
  .text : { 
    *(.text*)           /* Collects machine code of all functions from all compiled .o files */
  } > flash             /* Stored in FLASH memory */

  /* 3. Read-Only Data section (constants, string literals) */
  .rodata : { 
    *(.rodata*)         /* Immutable data */
  } > flash             /* Stored in FLASH memory */

  /* 4. Initialized global variables section (e.g., int counter = 42;) */
  .data : {
    _sdata = .;         /* Defines symbol _sdata — start address of .data in SRAM */
    *(.first_data)      /* High-priority data (if any) */
    *(.data SORT(.data.*)) /* Collects all global variables with initial values */
    _edata = .;         /* Defines symbol _edata — end address of .data in SRAM */
  } > sram AT > flash
  /* * CRITICAL NOTE "> sram AT > flash":
   * At runtime, variables must reside and change in RAM (> sram).
   * But their initial values (e.g., number 42) must be stored in FLASH (AT > flash),
   * otherwise initial values would be lost when powered off!
   */

  /* * Gets the physical address in FLASH memory where initial bytes for .data reside.
   * Our _reset() function will copy data from _sidata (Flash) to _sdata (SRAM).
   */
  _sidata = LOADADDR(.data);

  /* 5. Uninitialized global variables section (e.g., int status;) */
  .bss : {
    _sbss = .;          /* Symbol for the start of .bss section in SRAM */
    *(.bss SORT(.bss.*) COMMON) /* Collects all variables that need to be zeroed */
    _ebss = .;          /* Symbol for the end of .bss section in SRAM */
  } > sram
  /* * .bss is located only in SRAM (> sram) and takes NO space in FLASH!
   * Our _reset() function simply zeroes out this memory region on startup.
   */

  /* Align the current location counter (represented by '.') to an 8-byte boundary (ARM ABI rule) */
  . = ALIGN(8);

  /* * End of allocated memory symbol.
   * Used by standard C libraries to determine the start of dynamic memory (Heap allocation)
   */
  _end = .;             
}