# ESP32-S3 Pin Assignments for Rover Motor Control

## Default Pin Configuration (Config.hpp)

```
╔═══════════════════════════════════════════════════════════════╗
║                    ESP32-S3-DEVKIT                           ║
║                                                               ║
║  ┌─────────────────────────────────────────────────────┐    ║
║  │                                                       │    ║
║  │  GPIO 4  ──────► LEFT_MOTOR_PWM   (M1 on MDD3A)      │    ║
║  │  GPIO 5  ──────► LEFT_MOTOR_DIR   (M1 on MDD3A)      │    ║
║  │  GPIO 6  ──────► RIGHT_MOTOR_PWM  (M2 on MDD3A)      │    ║
║  │  GPIO 7  ──────► RIGHT_MOTOR_DIR  (M2 on MDD3A)      │    ║
║  │                                                       │    ║
║  │  GPIO 18 (TX) ──► Raspberry Pi RX                    │    ║
║  │  GPIO 16 (RX) ──► Raspberry Pi TX                    │    ║
║  │  GND         ───► Common Ground                      │    ║
║  │                                                       │    ║
║  └─────────────────────────────────────────────────────┘    ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

## MDD3A Motor Driver Connections

```
╔═══════════════════════════════════════════════════════════════╗
║                    MDD3A Motor Driver                        ║
║                                                               ║
║  ┌─────────────────────────────────────────────────────┐    ║
║  │  MOTOR 1 (LEFT)                  MOTOR 2 (RIGHT)    │    ║
║  │                                                       │    ║
║  │  PWM  ──► GPIO 4                 PWM  ──► GPIO 6     │    ║
║  │  DIR  ──► GPIO 5                 DIR  ──► GPIO 7     │    ║
║  │                                                       │    ║
║  │  M1+  ──► Left Motor (+)        M2+  ──► Right Motor(+)│    ║
║  │  M1-  ──► Left Motor (-)        M2-  ──► Right Motor(-)│    ║
║  │                                                       │    ║
║  │  VCC  ──► +12V Power Supply                         │    ║
║  │  GND  ──► Common Ground                             │    ║
║  │                                                       │    ║
║  └─────────────────────────────────────────────────────┘    ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

## Power Distribution

```
                    ╔═══════════════════════════════╗
                    ║   12V DC Power Supply        ║
                    ╚═══════════════════════════════╝
                             │
            ┌────────────────┴────────────────┐
            │                                 │
            ▼                                 ▼
    ╔═══════════════╗               ╔═══════════════╗
    ║   MDD3A VCC   ║               ║   ESP32-S3    ║
    ║   (12V Input) ║               ║   (5V or USB) ║
    ╚═══════════════╝               ╚═══════════════╝
            │                                 │
            └────────────────┬────────────────┘
                             ▼
                      ╔═══════════════╗
                      ║  COMMON GND    ║
                      ╚═══════════════╝
```

## PWM Specifications

- **PWM Frequency**: 20 kHz (set in MotorDriver.cpp)
- **PWM Resolution**: 8-bit (0-255)
- **PWM Channels**: LED PWM channel 0 (shared between motors)

## Signal Logic

```
Motor Speed Control:
- PWM duty cycle: 0-255 (0% to 100% speed)
- Direction pin: HIGH = forward, LOW = backward

Speed to PWM Conversion:
- Input: -100 to +100 (percentage)
- PWM = abs(speed) * 255 / 100
- Direction = speed >= 0

Examples:
- speed = 50   → PWM = 127, DIR = HIGH
- speed = -50  → PWM = 127, DIR = LOW
- speed = 100  → PWM = 255, DIR = HIGH
- speed = 0    → PWM = 0,   DIR = doesn't matter
```

## Alternative Pin Options

If default pins (4-7) don't work for your setup, ESP32-S3 PWM-capable pins include:
- GPIO 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21
- GPIO 35, 36, 37, 38, 39, 40, 41, 42, 47, 48

Update `Config.hpp` to change pin assignments.

## Serial Communication Pins

Default UART pins on ESP32-S3 (configured in Config.hpp):
- **GPIO 18**: TX (transmit to Raspberry Pi RX)
- **GPIO 16**: RX (receive from Raspberry Pi TX)

These can be changed by modifying `Config.hpp` UART_PIN configuration.