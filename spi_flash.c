#include "spi_flash.h"
#include "rtc.h"
#include "global.h"

// Put the data in the right bit endianess to hardware
static void shift_out_byte(uint8_t data) {
    // Bit order is flipped
    for (int i = 7; i >= 0; i--) {
        // Get bit of data to write
        char curr_data = data >> i & 1;
        
        // Write bit on the wire
        if (curr_data) {
            SET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_MOSI);
        } else {
            UNSET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_MOSI);
        }

        // Toggle CLK to get hardware to eat byte   
        SET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_SCK);
        delay_us(2);
        UNSET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_SCK);
        delay_us(2);
    }
}

// Read data in the right bit endianess from hardware
static uint8_t shift_in_byte(void) {
    // Bit order is flipped
    uint8_t read_byte = 0;
    for (int i = 7; i >= 0; i--) {
        // Toggle CLK to get hardware to spit out byte
        SET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_SCK);
        delay_us(2);

        // Read bit of data and build out read data
        if (FLASH_SPI_PORT_DATA & FLASH_SPI_MISO) {
            read_byte = (read_byte << 1) | 1;
        } else {
            read_byte = (read_byte << 1);
        }

        // Finished reading our one bit
        UNSET_BIT(FLASH_SPI_PORT_DATA, FLASH_SPI_SCK);
        delay_us(2);
    }
    return read_byte;
}


uint32_t spi_flash_init(void) {
    disable_irqs();
    // no DIR bit for SPI_MISO — leave as input
    #define SPI_PORT_AFSEL  (*(volatile uint32_t *)(FLASH_SPI_PORT_BASE + 0x420))
    UNSET_BIT(SPI_PORT_AFSEL, FLASH_SPI_MISO);
    
    SET_BIT(FLASH_SPI_PORT_DEN, FLASH_SPI_MISO);
    UNSET_BIT(FLASH_SPI_PORT_DIR, FLASH_SPI_MISO);

    SET_BIT(FLASH_LATCH_PORT_DEN, FLASH_PIN_LATCH);
    SET_BIT(FLASH_LATCH_PORT_DIR, FLASH_PIN_LATCH);

    delay_us(2);

    // Pull low to send bytes
    UNSET_BIT(FLASH_LATCH_PORT_DATA, FLASH_PIN_LATCH);
    delay_us(2);

    // TX byte
    shift_out_byte(0x9F);

    // RX bytes
    uint32_t tmp = 0;
    tmp = shift_in_byte();
    tmp = (tmp << 8) | shift_in_byte();
    tmp = (tmp << 8) | shift_in_byte();

    // Pull high to undo CS
    SET_BIT(FLASH_LATCH_PORT_DATA, FLASH_PIN_LATCH);

    enable_irqs();
    return tmp;
}