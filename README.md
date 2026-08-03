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

### Getting JTAG Working On The UNI-DS-8 with this chip

#### JLink Plus Connector

```text
      ┌───────────────┐
  1 ● │ VTref     GND │ ● 2
  3 ● │ nTRST     GND │ ● 4
  5 ● │ TDI       GND │ ● 6
  7 ● │ TMS       GND │ ● 8
  9 ● │ TCK       GND │ ●10
 11 ● │ RTCK      GND │ ●12
 13 ● │ TDO       GND │ ●14
 15 ● │ nRESET    GND │ ●16
 17 ● │ DBGRQ     GND │ ●18
 19 ● │ +5V       GND │ ●20
      └───────────────┘
```

#### UNI-DS-8 External Debug Connect

```text
   CN10
  ┌───┐
1 | ● | RESET
2 | ● | VCC
3 | ● | GND
4 | ● | SWO/TDO/MISO/PGD
5 | ● | SWDCLK/TCK/SCK/PGC
6 | ● | RESET (nRST)
7 | ● | TDI
8 | ● | SWDIO/TMS/MOSI
  └───┘
```

#### Connecting the Connectors

```txt
CN10 1 -> JLink 3
CN10 2 -> JLink 1
CN10 3 -> Jlink 4
CN10 4 -> Jlink 13
CN10 5 -> Jlink 9
CN10 6 -> Jlink 15
CN10 7 -> Jlink 5
CN10 8 -> Jlink 7
```

#### Other JTAG Notes

There is a switch for External/Onboard in the `DBG SEL` region of the UNI-DS-8, it has a thin layer of some sheath on top of it that must be peeled off to access the switch. After flipping the switch, ensure the board is rebooted before attempting a connection.