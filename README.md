# openag_gc0011
A derivative of [openag_gc0012](https://github.com/OpenAgInitiative/openag_gc0012)

OpenAg firmware for the COZIR GC-0011 Carbon dioxide sensor.

Specifically written for the PFC1 BOM. 

This code uses specifiable pins with Software Serial instead of hardware serial ports on the Arduino Mega as with [openag_gc0012](https://github.com/OpenAgInitiative/openag_gc0012).

Supports:
- GC-0010
- GC-0011
- GC-0012

## Status Messages

- "31": Failed to read from sensor
