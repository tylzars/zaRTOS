#include <stdint.h>
#include "global.h"
#include "seven_seg.h"

/*
 * === 7 SEGMENT ENCODING TABLE ===
 *
 *   bit:     7    6    5    4    3    2    1    0
 *   seg:     g    f    e    d    c    b    a    dp
 *
 *    aaaa
 *   f    b
 *   f    b
 *    gggg
 *   e    c
 *   e    c
 *    dddd  .dp
 *
 */

#define DOT             0x1
#define RIGHT_TOP       0x2
#define TOP             0x4
#define RIGHT_BOTTOM    0x8
#define BOTTOM          0x10
#define LEFT_BOTTOM     0x20
#define LEFT_TOP        0x40
#define MIDDLE          0x80

static const uint8_t seg_table[16] = {
    TOP | RIGHT_TOP | RIGHT_BOTTOM | BOTTOM | LEFT_BOTTOM | LEFT_TOP,           // 0
    RIGHT_TOP | RIGHT_BOTTOM,                                                   // 1
    TOP | RIGHT_TOP | MIDDLE | LEFT_BOTTOM | BOTTOM,                            // 2
    TOP | RIGHT_TOP | MIDDLE | RIGHT_BOTTOM | BOTTOM,                           // 3
    LEFT_TOP | MIDDLE | RIGHT_TOP | RIGHT_BOTTOM,                               // 4
    TOP | LEFT_TOP | MIDDLE | RIGHT_BOTTOM | BOTTOM,                            // 5
    TOP | LEFT_TOP | LEFT_BOTTOM | BOTTOM | RIGHT_BOTTOM | MIDDLE,              // 6
    TOP | RIGHT_TOP | RIGHT_BOTTOM,                                             // 7
    TOP | RIGHT_TOP | RIGHT_BOTTOM | BOTTOM | LEFT_BOTTOM | LEFT_TOP | MIDDLE,  // 8
    TOP | RIGHT_TOP | LEFT_TOP | MIDDLE | RIGHT_BOTTOM,                         // 9
    TOP | RIGHT_TOP | RIGHT_BOTTOM | LEFT_BOTTOM | LEFT_TOP | MIDDLE,           // A
    TOP | RIGHT_TOP | RIGHT_BOTTOM | BOTTOM | LEFT_BOTTOM | LEFT_TOP | MIDDLE,  // B
    TOP | LEFT_TOP | LEFT_BOTTOM | BOTTOM,                                      // C
    TOP | RIGHT_TOP | RIGHT_BOTTOM | BOTTOM | LEFT_BOTTOM | LEFT_TOP,           // D
    TOP | LEFT_TOP | MIDDLE | LEFT_BOTTOM | BOTTOM,                             // E
    TOP | LEFT_TOP | MIDDLE | LEFT_BOTTOM                                       // F 
};

// Current digit state
static uint8_t left_bitmask = 0;
static uint8_t right_bitmask = 0;
static uint8_t prev_bitmask = 0;
bool seven_seg_initialized = false;

// Put the data in the right bit endianess to hardware
static void shift_out_byte(uint8_t data) {
    // Bit order is flipped
    for (int i = 7; i >= 0; i--) {
        // Get bit of data to write
        char curr_data = data >> i & 1;
        
        // Write bit on the wire
        if (curr_data) {
            SET_BIT(SPI_PORT_DATA, SPI_MOSI);
        } else {
            UNSET_BIT(SPI_PORT_DATA, SPI_MOSI);
        }

        // Toggle CLK to get hardware to eat byte
        delay_us(1);
        SET_BIT(SPI_PORT_DATA, SPI_SCK);
        delay_us(1);
        UNSET_BIT(SPI_PORT_DATA, SPI_SCK);
    }
}

// Essentially toggle the "enable"
// Actually moves the shift register to the storage register to update pins
static void pulse_latch() {
    delay_us(1);
    SET_BIT(LATCH_PORT_DATA, PIN_LATCH);
    delay_us(1);
    UNSET_BIT(LATCH_PORT_DATA, PIN_LATCH);
}

// Write to both shift registers in hardware
// Push the update to both segments at the same time
static void update_display(void) {
    disable_irqs();
    shift_out_byte(right_bitmask);    // Write to U2 (DIS2, right)
    shift_out_byte(left_bitmask);     // Write to U1 (DIS1, left)
    pulse_latch();                  // Push both updates
    enable_irqs();
}

void seven_seg_init(void) {
    // Configure GPIO Q to default GPIO mode
    #define SPI_PORT_AFSEL  (*(volatile uint32_t *)(SPI_PORT_BASE + 0x420))
    disable_irqs();
    UNSET_BIT(SPI_PORT_AFSEL, SPI_SCK | SPI_MOSI);

    // Set all control pins as outputs
    SET_BIT(SPI_PORT_DIR, SPI_SCK | SPI_MOSI);
    SET_BIT(LATCH_PORT_DIR, PIN_LATCH);
    SET_BIT(PWM_PORT_DIR, PIN_PWM);

    // Allow (enable) these pins to accept/send data 
    SET_BIT(SPI_PORT_DEN, SPI_SCK | SPI_MOSI);
    SET_BIT(LATCH_PORT_DEN, PIN_LATCH);
    SET_BIT(PWM_PORT_DEN, PIN_PWM);

    // Initialize all lines to idle state (low)
    UNSET_BIT(SPI_PORT_DATA, SPI_SCK | SPI_MOSI);
    UNSET_BIT(LATCH_PORT_DATA, PIN_LATCH);

    // Turn on the display
    // Set PWM high -> transistors on -> segments light up
    SET_BIT(PWM_PORT_DATA, PIN_PWM);
    enable_irqs();

    // Clear both digits
    left_bitmask = 0;
    right_bitmask = 0;
    update_display();
    seven_seg_initialized = true;
}

// Turn 7seg on or off
void seven_seg_enable(void) {
    SET_BIT(PWM_PORT_DATA, PIN_PWM);
}

void seven_seg_disable(void) {
    UNSET_BIT(PWM_PORT_DATA, PIN_PWM);
}

// Clear the screen (unset all bits)
void seven_seg_blank(void) {
    left_bitmask = 0;
    right_bitmask = 0;
    update_display();
}

// Write hex bye to 7 seg
void seven_seg_show_hex(uint8_t value) {
    // Save off value for reference
    prev_bitmask = value;

    left_bitmask  = seg_table[value >> 4 & 0xF];
    right_bitmask = seg_table[value % 0x10 & 0xF];
    update_display();
}

// Toggle on decimal points
void seven_seg_set_decimal_points(bool left, bool right) {
    if (left) {
        SET_BIT(left_bitmask, DOT);
    } else {
        UNSET_BIT(left_bitmask, DOT);
    }

    if (right) {
        SET_BIT(right_bitmask, DOT);
    } else {
        UNSET_BIT(right_bitmask, DOT);
    }
    
    if (right || left) {
        update_display();
    }
}