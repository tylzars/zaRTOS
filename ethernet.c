#include "ethernet.h"

void init_mac(void) {
    // Enable MAC
    SET_BIT(SYSCTL_RCGCEMAC, BIT(0));
    // Wait for MAC to come up
    while (SYSCTL_PREMAC != 0x1) {};
    // TODO!!!
    // 20.5 Initialization and Configuration
    return;
}