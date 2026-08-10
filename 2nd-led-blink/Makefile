# --- Path settings ---
BUILD_DIR = build
LINKER_SCRIPT = ../common/f446.ld
SOURCES   = src/main.c
TARGET    = $(BUILD_DIR)/firmware

# --- Compilation's flags ---
CFLAGS  ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
           -Wformat-truncation -fno-common -Wconversion \
           -g3 -O0 -ffunction-sections -fdata-sections -I. -Isrc \
           -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

# --- Linker Flags ---
LDFLAGS ?= -T$(LINKER_SCRIPT) -nostartfiles -nostdlib -lgcc \
           -Wl,--gc-sections -Wl,-Map=$(TARGET).map

# --- Main Goals ---
all: $(TARGET).bin

# Create /build 
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Making .elf
$(TARGET).elf: $(SOURCES) | $(BUILD_DIR)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Convert .elf to clean .bin
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# Programming into Mikrochip with st-flash
flash: $(TARGET).bin
	st-flash --reset write $< 0x08000000

# Clean Project
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean flash