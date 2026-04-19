# Rover Wiring Guide

## ESP32-S3 to MDD3A Motor Driver Connections

```
ESP32-S3                    MDD3A Motor Driver
────────────────────────────────────────────────────────────
GPIO 4 (PWM)    -------->   M1 PWM (Speed Control)
GPIO 5 (DIR)    -------->   M1 DIR (Direction)
GPIO 6 (PWM)    -------->   M2 PWM (Speed Control)
GPIO 7 (DIR)    -------->   M2 DIR (Direction)
GND             -------->   GND (Common Ground)
```

## MDD3A to Motors Connections

```
MDD3A Motor Driver         Motors
────────────────────────────────────────────────────────────
M1 (+)            -------->   Left Motor (+)
M1 (-)            -------->   Left Motor (-)
M2 (+)            -------->   Right Motor (+)
M2 (-)            -------->   Right Motor (-)
```

## Power Connections

```
Power Supply (12V)          MDD3A Motor Driver
────────────────────────────────────────────────────────────
Positive (+)       -------->   VCC (12V Input)
Negative (-)       -------->   GND

ESP32-S3                    Power Supply (5V or USB)
────────────────────────────────────────────────────────────
5V or VIN         -------->   5V (+)
GND               -------->   GND (-)

IMPORTANT: Connect all GND together:
- ESP32 GND ----> MDD3A GND
- MDD3A GND ----> Power Supply GND
- ESP32 GND ----> Power Supply GND (if separate supplies)
```

## Serial Communication (ESP32-S3 ↔ Raspberry Pi)

```
ESP32-S3                    Raspberry Pi 5
────────────────────────────────────────────────────────────
GPIO 18 (TX)      -------->   RX (GPIO 15 - UART0 RX)
GPIO 16 (RX)      -------->   TX (GPIO 14 - UART0 TX)
GND               -------->   GND
```

**Note**: ESP32-S3 GPIO 18 is TX (transmit), GPIO 16 is RX (receive).
Ensure TX connects to RX and RX connects to TX (crossed connection).

## Notes:
1. Use appropriate gauge wire for motor connections (18-22 AWG recommended)
2. Keep motor power separate from logic power to avoid interference
3. Add decoupling capacitors (100µF) near motor power inputs if needed
4. Ensure all grounds are connected together
5. Motor direction can be reversed by swapping +/- connections or setting invertDirection in code
6. PWM frequency is set to 20kHz for smooth motor operation
7. Maximum current per channel: 3A (continuous)
8. Test with low speeds first to verify correct motor direction

## Safety:
- Double-check all connections before applying power
- Start with low speeds and verify motor rotation direction
- Ensure secure connections to prevent loose wires during operation
- Consider adding fuse protection on motor power supply
- Keep electronics away from moving parts