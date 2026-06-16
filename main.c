#include <stdint.h>
#include "global.h"
#include "lcd.h"
#include "mlib.h"
#include "rtc.h"
#include "seven_seg.h"
#include "timer.h"
#include "round_robin.h"
#include "ethernet.h"

uint32_t loop = 0xe000e100;

void hexdumper(void) {
    char read_bytes[17] = "\0";
    char hex_addr[17] = "\0";

    while (true){
        m_memset(hex_addr, 0, 17);
        m_memset(read_bytes, 0x0, 17);
        
        m_sprintf(hex_addr, "%X\n", loop);

        char* p = read_bytes;
        p += m_sprintf(p, "%x", *(uint32_t*)(loop));
        p += m_sprintf(p, "%hx", *(uint16_t*)(loop+4));
        p += m_sprintf(p, "%hhx", *(uint8_t*)(loop+6));
        p += m_sprintf(p, "%hhx", *(uint8_t*)(loop+7));
        *p = '\0';
        
        disable_irqs();
        lcd_put_string(hex_addr);
        lcd_put_string(read_bytes);
        enable_irqs();
    }
}

void timerman(void) {
    while (true) {
        seven_seg_set_decimal_points(false, true);
    
        // Let timer rip
        start_timer(TIMER0, 32000000);

        // Check if timer finished
        extern bool timer0_triggered;
        if(timer0_triggered) {
            seven_seg_show_hex((((loop - 0xe000e100) & 0xFF) % 0xFF));
            timer0_triggered = false;
        }
    }
}

void round_robin_reset(void) {
    while (true) {
        loop += 8;
        lcd_clear_screen();
        sleep_s(6);
    }
}

int main() {
    // TODO: Move all INIT code into one file (maybe bootloader?)
    enable_hibernation();
    enable_rtc();

    delay_ms(20);
    lcd_init();
    lcd_set_display_on_off(true, false, true);
    lcd_set_entry_mode(true, false);
    
    seven_seg_init();
    seven_seg_blank();
    sleep_s(1);
    //seven_seg_show_hex(0x33);
    seven_seg_set_decimal_points(false, true);

    // Setup MAC/PHY
    init_mac();

    // NVIC Enables (3.4)
    if ((NVIC->isr_en0 & (1 << 19)) == 0) {
        SET_BIT(NVIC->isr_en0, TIMER0_ISR);    
    }

    enable_timer(0);

    // Tasks setup
    create_task((void*)hexdumper);
    create_task((void*)timerman);   
    create_task((void*)round_robin_reset);
    current_tcb = &tasks[0];
    next_tcb = &tasks[0];
    curr_task = 0;

    // Enable systick
    SYSTICK->STRELOAD = 0x00FFFFFF; // TODO: Saleae this out to find duration
    SYSTICK->STCURRENT = 0x00FFFFFF;
    SET_BIT(SYSTICK->STCTRL, BIT(0) | BIT(1));

    // Start round robin
    //asm volatile("MSR PSP, %0" : : "r" (current_tcb->sp) : );
    scheduler_launch();

    while(true) {}

    return 0;
}