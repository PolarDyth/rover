# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Physical rover project with dual-processor architecture:
- **Raspberry Pi 5** (pi-controller): High-level control, decision making, and serial communication
- **ESP32-S3-DEVKIT** (esp32-firmware): Real-time motor control and hardware interfacing
- **Communication**: Serial UART between Pi and ESP32 via `/dev/ttyAMA0` at 115200 baud
- **Hardware**: 2x 12V DC Motors with MDD3A 3Amp 4V-16V DC Motor Driver, DC power supply

## Architecture

### pi-controller (C++ Application)
- **Location**: `pi-controller/`
- **Purpose**: Main control application running on Raspberry Pi 5
- **Key Components**:
  - `SerialPort`: Handles serial communication with ESP32 via Linux termios API
  - Currently sends simple PING commands, will expand to motor control commands
- **Build System**: CMake with C++17 standard
- **Runtime**: Direct execution on Raspberry Pi OS

### esp32-firmware (PlatformIO Project)
- **Location**: `esp32-firmware/`
- **Purpose**: Motor control firmware for ESP32-S3
- **Build System**: PlatformIO (Arduino framework)
- **Key Components**:
  - `MotorDriver`: Controls MDD3A motor driver with PWM and direction pins
  - `CommandProtocol`: Parses and executes serial commands from Raspberry Pi
  - Serial command interface for motor control (MOVE, STOP, SET, PING, etc.)
  - Watchdog timer for automatic motor stopping on communication loss
- **Pin Configuration**: See `Config.hpp` for default pin assignments (GPIO 4-7 for motors)

## Common Commands

### Building Pi Controller
```bash
cd pi-controller
cmake -B build
cmake --build build
```

### Running Pi Controller
```bash
./pi-controller/build/pi_controller
```

### Building ESP32 Firmware
```bash
cd esp32-firmware
pio run
```

### Uploading ESP32 Firmware (Has to be done on windows)
```bash
cd esp32-firmware
pio run --target upload
```

### Monitoring ESP32 Serial Output
```bash
cd esp32-firmware
pio device monitor
```

## Hardware Interface Details

- **Serial Port**: `/dev/ttyAMA0` (Raspberry Pi UART to ESP32)
- **Baud Rate**: 115200
- **UART Pins**: ESP32-S3 GPIO 16 (RX), GPIO 18 (TX) - configured in `Config.hpp`
- **Motor Driver**: MDD3A controls 2 DC motors via PWM and direction pins
- **Motor Pins**: GPIO 4-7 (PWM and direction for left/right motors)
- **Power**: 12V DC power supply for motors, separate power for logic boards

## Development Notes

- Serial communication uses 8N1 format (8 data bits, no parity, 1 stop bit)
- SerialPort class handles line-oriented protocol with newline termination
- ESP32 firmware responds to Pi commands and handles real-time motor control
- Both components maintain protocol compatibility for command/response messaging

## Serial Command Protocol

The ESP32 firmware accepts commands via serial at 115200 baud:
- `MOVE <direction> [speed]` - Control rover movement (0=forward, 2=backward, 3=left, 4=right, 5=rotate_left, 6=rotate_right)
- `STOP` - Stop all motors immediately
- `SET <left_speed> <right_speed>` - Set individual motor speeds (-100 to 100)
- `PING` - Test communication
- `STATUS` - Get controller status
- `HELP` - Show available commands

Response format: `STATUS:message` (e.g., `OK:Moving forward at speed 50`)

## Safety Features

- Watchdog timer stops motors if no commands received for 1 second
- All speeds automatically constrained to safe ranges
- Emergency stop via STOP command or watchdog timeout