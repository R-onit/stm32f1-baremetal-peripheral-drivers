EXAMPLE ?= 00_led_toggle-addr
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
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
CFLAGS := -mcpu=cortex-m3 -mthumb -g \
          -Idrivers/inc \
          -Iexamples/$(EXAMPLE) \
          -Icmsis/Include \
          -Icmsis/Device

OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32f1x.cfg
OPENOCD = openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET)

LDFLAGS := -T $(LINKER_SCRIPT) -nostartfiles

# output files
ELF := $(BUILD_DIR)/firmware.elf
BIN := $(BUILD_DIR)/firmware.bin

# SOURCE FILES 
SRC := $(wildcard $(EXAMPLE_DIR)/*.c) \
       $(wildcard $(DRIVERS_SRC)/*.c)

OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ += $(OBJ_DIR)/startup_stm32f103rbtx.o

# BUILD RULES 

# Default target
all: $(BIN)

# making build directories 
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/$(EXAMPLE_DIR)
	@mkdir -p $(OBJ_DIR)/$(DRIVERS_SRC)
	@mkdir -p $(BUILD_DIR)

# Compiling C source files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Compiling assembly startup
$(OBJ_DIR)/startup_stm32f103rbtx.o: $(STARTUP)
	@$(CC) $(CFLAGS) -c $< -o $@

# Linkinking objects into ELF
$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

# Convert ELF to BIN
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

size :
	$(SIZE) $(OBJ_DIR)/$(EXAMPLE_DIR)/main.o
dump :
	$(OBJDUMP) -d $(OBJ_DIR)/$(EXAMPLE_DIR)/main.o | $(OBJ_DIR)/*.o > dump.txt 

# obj/examples/00_led_toggle-addr/main.o
# Clean
clean:
	rm -rf build dump.txt

flash: $(ELF)
	@echo "Flashing firmware to STM32..."
	@$(OPENOCD) -c "program $(ELF) verify reset exit"

# Start OpenOCD server (for GDB connection)
openocd:
	@echo "Starting OpenOCD GDB server..."
	@$(OPENOCD)

# GDB session
gdb:
	@echo "Connecting to GDB..."
	@gdb-multiarch $(ELF) -ex "target remote localhost:3333"

list:
	@echo "Available examples:"
	@ls examples | nl


.PHONY: flash gdb clean list all

