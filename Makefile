PLATFORM :=
ifeq ($(OS),Windows_NT)
    PLATFORM := windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := osx
    else ifeq ($(UNAME_S),Linux)
        PLATFORM := linux
    endif
endif

MCU             =   attiny1616
MCU_FLASH_SIZE  =   16384
MCU_SRAM_SIZE   =   2048
MCU_EEPROM_SIZE =   256

F_CPU           =   32768UL

CC              =   toolchain/$(PLATFORM)/avr/bin/avr-gcc
SIZE            =   toolchain/$(PLATFORM)/avr/bin/avr-size
OBJCOPY         =   toolchain/$(PLATFORM)/avr/bin/avr-objcopy

CFLAGS          =   -Wall \
                    -Os \
                    -DF_CPU=$(F_CPU) \
                    -mmcu=$(MCU) \
                    -Iinclude \
                    -Iinclude/drivers

LDFLAGS         =   -mmcu=$(MCU)

TARGET          =   out/main
SOURCES         =   src/main.c \
                    drivers/driver_system.c \
                    drivers/driver_boost.c \
                    drivers/driver_secondary.c \
                    drivers/driver_voltageTemperature.c

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	@echo ""
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).elf: $(SOURCES)
	@echo ""
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

size: $(TARGET).elf
	@echo ""
	@FLASH=$$($(SIZE) -A $(TARGET).elf | grep -E '.text|.data' | awk '{sum += $$2} END {print sum}'); \
	 SRAM=$$($(SIZE) -A $(TARGET).elf | grep -E '.data|.bss' | awk '{sum += $$2} END {print sum}'); \
	 EEPROM=$$($(SIZE) -A $(TARGET).elf | grep -E '.eeprom' | awk '{print $$2}'); if [ -z "$$EEPROM" ]; then EEPROM=0; fi;\
	 FLASH_PCT=$$(echo "scale=1; 100 * $$FLASH / $(MCU_FLASH_SIZE)" | bc); \
	 SRAM_PCT=$$(echo "scale=1; 100 * $$SRAM / $(MCU_SRAM_SIZE)" | bc); \
	 EEPROM_PCT=$$(echo "scale=1; 100 * $$EEPROM / $(MCU_EEPROM_SIZE)" | bc); \
	 printf "%-7s  %5s bytes  %5.1f %%\n" "FLASH:" $$FLASH $$FLASH_PCT; \
	 printf "%-7s  %5s bytes  %5.1f %%\n" "SRAM:" $$SRAM $$SRAM_PCT; \
	 printf "%-7s  %5s bytes  %5.1f %%\n" "EEPROM:" $$EEPROM $$EEPROM_PCT;
	@echo ""

clean:
	rm -f $(TARGET).hex $(TARGET).elf
