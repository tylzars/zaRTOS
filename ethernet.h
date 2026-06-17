#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include "global.h"

#define SYSCTL_RCGCEMAC (*(__IOM uint32_t*)0x400FE69C)
#define SYSCTL_PREMAC (*(__IM uint32_t*)0x400FEA9C)

#define EMACDMABUSMOD (*(__IOM uint32_t*)0x400ECC00)

// Functionality
void init_mac(void);

#endif // ETHERNET_H