#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include <stdint.h>
#include "global.h"

// PORT CONFIGURATION — mikroBUS1 on SiBRAIN TM4C1294NCPDT

/*
 * SPI DATA + CLOCK: Port Q
 * Port Q AHB base: 0x40066000
 */
#define SPI_PORT_BASE     0x40066000
#define SPI_PORT_DATA     (*(volatile uint32_t*)(SPI_PORT_BASE + 0x3FC))
#define SPI_PORT_DIR      (*(volatile uint32_t*)(SPI_PORT_BASE + 0x400))
#define SPI_PORT_DEN      (*(volatile uint32_t*)(SPI_PORT_BASE + 0x51C))

#define SPI_MOSI    (1 << 2)
#define SPI_SCK     (1 << 0)

/*
 * LATCH: Port H
 * Port H AHB base: 0x4005F000
 */
#define LATCH_PORT_BASE    0x4005F000
#define LATCH_PORT_DATA    (*(volatile uint32_t*)(LATCH_PORT_BASE + 0x3FC))
#define LATCH_PORT_DIR     (*(volatile uint32_t*)(LATCH_PORT_BASE + 0x400))
#define LATCH_PORT_DEN     (*(volatile uint32_t*)(LATCH_PORT_BASE + 0x51C))

#define PIN_LATCH  (1 << 0)   // PH0

/*
 * PWM: Port L
 * Port L AHB base: 0x40062000
 * We use this as a simple digital on/off for now.
 */
#define PWM_PORT_BASE      0x40062000
#define PWM_PORT_DATA      (*(volatile uint32_t*)(PWM_PORT_BASE + 0x3FC))
#define PWM_PORT_DIR       (*(volatile uint32_t*)(PWM_PORT_BASE + 0x400))
#define PWM_PORT_DEN       (*(volatile uint32_t*)(PWM_PORT_BASE + 0x51C))

#define PIN_PWM    (1 << 4)   // PL4

// Functionality
void seven_seg_init(void);
void seven_seg_disable(void);
void seven_seg_enable(void);
void seven_seg_blank(void);
void seven_seg_show_hex(uint8_t value);
void seven_seg_set_decimal_points(bool left, bool right);

#endif // SEVEN_SEG_H
