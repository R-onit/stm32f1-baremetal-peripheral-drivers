# ====== CONFIGURATION ======
EXAMPLE ?= 00_LED_TOGGLE-addr
TARGET := $(EXAMPLE)

# directories
BUILD_DIR := build/$(TARGET)
OBJ_DIR := $(BUILD_DIR)/obj
DRIVERS_SRC := drivers/src
DRIVERS_INC := drivers/inc
EXAMPLE_DIR := examples/$(TARGET)
STARTUP := startup/startup_stm32f103rbtx.s
LINKER_SCRIPT := STM32F103RBTX_FLASH.ld

# toolchain
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
CFLAGS := -mcpu=cortex-m3 -mthumb -g \
          -Idrivers/inc \
          -Iexamples/$(EXAMPLE) \
          -Icmsis/Include \
          -Icmsis/Device

LDFLAGS := -T $(LINKER_SCRIPT) -nostartfiles

# output files
ELF := $(BUILD_DIR)/firmware.elf
BIN := $(BUILD_DIR)/firmware.bin

# ====== SOURCE FILES ======
SRC := $(wildcard $(EXAMPLE_DIR)/*.c) \
       $(wildcard $(DRIVERS_SRC)/*.c)

OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ += $(OBJ_DIR)/startup_stm32f103rbtx.o

# ====== BUILD RULES ======

# Default target
all: $(BIN)

# Ensure build directories exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/$(EXAMPLE_DIR)
	@mkdir -p $(OBJ_DIR)/$(DRIVERS_SRC)
	@mkdir -p $(BUILD_DIR)

# Compile C source files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly startup
$(OBJ_DIR)/startup_stm32f103rbtx.o: $(STARTUP)
	$(CC) $(CFLAGS) -c $< -o $@

# Link objects into ELF
$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

# Convert ELF to BIN
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Clean
clean:
	rm -rf build
