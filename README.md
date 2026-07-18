# zaRTOS

This code is for a TM4C1294NCPDT in a Mikroe UNI-DS-V8.

Currently attached peripherals:

- 2x16 character screen (lcd 2x16 port)
- 7 segment display     (mikroBus 1) - Done
- SPI Flash             (mikroBus 2) - WIP
- EEPROM                (mikroBus 3) - Not Started

Use `make` to make a `main.hex` that can be flashed with CodeGrip Studio.

## Plan

~~- Implement systick~~

~~- Make adding RR item~~
~~  - Figure out context switch~~
~~- systick iterrupt swaps between rr items~~
- Shared data (semaphore/mutex) [stuck in atomic shenanigans]
~~- 1st -> 7seg updates / 2nd -> lcd update~~
- Add ethernet....

mutex and ethernet code is DoA...
