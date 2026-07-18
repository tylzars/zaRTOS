#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include <stdint.h>
#include "global.h"
/*
 * LATCH: Port H
 * Port H AHB base: 0x4005F000
 */
#define LATCH_PORT_BASE    0x4005F000
#define LATCH_PORT_DATA    (*(__IOM*)(LATCH_PORT_BASE + 0x3FC))
#define LATCH_PORT_DIR     (*(__IOM*)(LATCH_PORT_BASE + 0x400))
#define LATCH_PORT_DEN     (*(__IOM*)(LATCH_PORT_BASE + 0x51C))

#define PIN_LATCH  (1 << 1)   // PH1 (CS)

/*
 * SPI DATA + CLOCK: Port Q
 * Port Q AHB base: 0x40066000
 */
#define SPI_PORT_BASE     0x40066000
#define SPI_PORT_DATA     (*(__IOM*)(SPI_PORT_BASE + 0x3FC))
#define SPI_PORT_DIR      (*(__IOM*)(SPI_PORT_BASE + 0x400))
#define SPI_PORT_DEN      (*(__IOM*)(SPI_PORT_BASE + 0x51C))

#define SPI_MOSI    (1 << 2)
#define SPI_MISO    (1 << 3)
#define SPI_SCK     (1 << 0)

// Functionality
uint32_t spi_flash_init(void);

#endif // SPI_FLASH_H