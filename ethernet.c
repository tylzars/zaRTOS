#include "ethernet.h"

void init_mac(void) {
    // Enable MAC
    SET_BIT(SYSCTL_RCGCEMAC, BIT(0));
    // Wait for MAC to come up
    while (SYSCTL_PREMAC != 0x1) {};

    // Enable PHY
    SET_BIT(SYSCTL_RCGCEPHY, BIT(0));
    // Wait for PHY to come up
    while (SYSCTL_PREPHY != 0x1) {};
    
    // TODO!!!
    // 20.5 Initialization and Configuration

    // Wait for DMA to clear - TODO: BROKEN
    while ((EMACDMABUSMOD & 0x1) == 0x1) {} 
    
    // Set ATDS (Alternate Descriptor Size)
    // Set PBL (Programmable Burst Length) to 8
    // Set RPBL (RX Programmable Burst Length) to 8
    // Set USP (User Seperate Programmable Length)
    // Set AAL (Aligned Address Beats)
    SET_BIT(EMACDMABUSMOD, BIT(7) | BIT(11) | BIT(20) | BIT(23) | BIT(25));

    return;
}