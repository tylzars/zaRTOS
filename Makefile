CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
TARGET = main

# Source files
SRCS = main.c lcd.c global.c vector_table.c mlib.c rtc.c seven_seg.c timer.c round_robin.c ethernet.c

# Flags
CFLAGS_COMMON = -mcpu=cortex-m4 \
-mthumb \
-mfpu=fpv4-sp-d16 \
-mfloat-abi=hard \
-DPART_TM4C1294NCPDT \
-Wall \
-T linker.ld \
-nostdlib \
-nostartfiles \
--specs=nano.specs \
-Wl,--gc-sections \
-fno-builtin

CFLAGS_DEBUG = -O0 -g -DDEBUG
CFLAGS_RELEASE = -O2

# Default = release
BUILD ?= release

ifeq ($(BUILD),debug)
CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
else ifeq ($(BUILD),release)
CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
endif

# Outputs
ELF = $(TARGET).elf
HEX = $(TARGET).hex

# Build rules
all: $(HEX)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Convenience targets
debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

clean:
	rm -f $(ELF) $(HEX)