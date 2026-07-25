#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include <stdint.h>
#include "global.h"
/*
 * LATCH: Port H
 * Port H AHB base: 0x4005F000
 */
#define FLASH_LATCH_PORT_BASE    0x4005F000
#define FLASH_LATCH_PORT_DATA    (*(volatile uint32_t*)(FLASH_LATCH_PORT_BASE + 0x3FC))
#define FLASH_LATCH_PORT_DIR     (*(volatile uint32_t*)(FLASH_LATCH_PORT_BASE + 0x400))
#define FLASH_LATCH_PORT_DEN     (*(volatile uint32_t*)(FLASH_LATCH_PORT_BASE + 0x51C))

#define FLASH_PIN_LATCH  (1 << 1)   // PH1 (CS)

/*
 * SPI DATA + CLOCK: Port Q
 * Port Q AHB base: 0x40066000
 */
#define FLASH_SPI_PORT_BASE     0x40066000
#define FLASH_SPI_PORT_DATA     (*(volatile uint32_t*)(FLASH_SPI_PORT_BASE + 0x3FC))
#define FLASH_SPI_PORT_DIR      (*(volatile uint32_t*)(FLASH_SPI_PORT_BASE + 0x400))
#define FLASH_SPI_PORT_DEN      (*(volatile uint32_t*)(FLASH_SPI_PORT_BASE + 0x51C))

#define FLASH_SPI_MOSI    (1 << 2)
#define FLASH_SPI_MISO    (1 << 3)
#define FLASH_SPI_SCK     (1 << 0)

// Functionality
uint32_t spi_flash_init(void);

#endif // SPI_FLASH_H