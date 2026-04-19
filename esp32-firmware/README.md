# ESP32 Rover Motor Controller

Firmware for ESP32-S3-DEVKIT to control a 2-motor rover system using MDD3A motor driver.

## Hardware Setup

### Components
- ESP32-S3-DEVKIT
- MDD3A 3Amp 4V-16V DC Motor Driver
- 2x 12V DC Motors
- 12V DC Power Supply

### Wiring Connections

#### MDD3A Motor Driver to ESP32-S3
- **Left Motor**:
  - PWM Pin → ESP32 GPIO 4
  - Direction Pin → ESP32 GPIO 5
- **Right Motor**:
  - PWM Pin → ESP32 GPIO 6
  - Direction Pin → ESP32 GPIO 7

#### MDD3A Motor Driver to Motors
- **Motor 1 (Left)**: Connect to M1 outputs (+/-)
- **Motor 2 (Right)**: Connect to M2 outputs (+/-)

#### Power Connections
- **MDD3A Power**: Connect 12V DC power supply to VCC and GND
- **ESP32 Power**: Separate 5V power supply or USB power
- **Common Ground**: Connect ESP32 GND to MDD3A GND

#### Serial Communication
- **ESP32 TX → Raspberry Pi RX**: GPIO 18 (transmit)
- **ESP32 RX → Raspberry Pi TX**: GPIO 16 (receive)
- **Ground**: Connect grounds together

### Pin Configuration Notes
- Pins 4-7 are used for motor control and support PWM on ESP32-S3
- If you need to use different pins, update `Config.hpp` accordingly
- Ensure pins used support PWM (most GPIO pins on ESP32-S3 do)

## Building and Uploading

### Build Firmware
```bash
cd esp32-firmware
pio run
```

### Upload Firmware (from Windows)
```bash
cd esp32-firmware
pio run --target upload
```

### Monitor Serial Output
```bash
cd esp32-firmware
pio device monitor
```

## Command Protocol

The firmware accepts commands via serial interface at 115200 baud.

### Command Format
```
COMMAND param1,param2,...
```

### Available Commands

#### MOVE Command
Control rover movement primitives.
```
MOVE <direction> [speed]
```
- **Directions**: 0=forward, 2=backward, 3=left, 4=right, 5=rotate_left, 6=rotate_right
- **Speed**: -100 to 100 (optional, default 50)
- **Examples**:
  - `MOVE 0 70` - Move forward at 70% speed
  - `MOVE 2 50` - Move backward at 50% speed
  - `MOVE 3 60` - Turn left at 60% speed
  - `MOVE 5 80` - Rotate left in place at 80% speed

#### STOP Command
Stop all motors immediately.
```
STOP
```

#### SET Command
Set individual motor speeds directly.
```
SET <left_speed> <right_speed>
```
- **Speed range**: -100 to 100
- **Example**: `SET 50,30` - Left motor 50%, right motor 30%

#### PING Command
Test communication with motor controller.
```
PING
```

#### STATUS Command
Get current controller status.
```
STATUS
```

#### HELP Command
Display available commands.
```
HELP
```

## Response Format

All commands return responses in the format:
```
STATUS:message
```

- **OK**: Command executed successfully
- **ERROR**: Command failed or invalid parameters
- **PONG**: Response to PING command
- **READY**: Initialization/status messages

## Safety Features

### Watchdog Timer
- Motors automatically stop if no commands received for 1 second
- Can be disabled in `Config.hpp` by setting `ENABLE_WATCHDOG = false`

### Speed Limiting
- All speeds automatically constrained to -100% to +100%
- Direction and speed are safely bounded

### Emergency Stop
- Send `STOP` command to immediately halt all motors
- Watchdog provides automatic timeout stopping

## Configuration

Edit `include/Config.hpp` to customize:
- Pin assignments
- Serial baud rate
- Timeout settings
- Watchdog behavior
- Default speeds

## Testing

The Raspberry Pi controller (`pi-controller`) includes a comprehensive test program that demonstrates all motor control functions. Build and run it to test your rover:

```bash
cd pi-controller
cmake -B build
cmake --build build
./build/pi_controller
```

## Troubleshooting

### Motors Not Responding
- Check wiring connections to MDD3A driver
- Verify pin configuration matches your wiring
- Ensure power supply is connected and providing 12V
- Check serial communication is working (send PING command)

### Erratic Motor Behavior
- Verify common ground between ESP32 and MDD3A
- Check for loose connections
- Ensure PWM frequency is appropriate for your motors

### Serial Communication Issues
- Confirm baud rate matches (115200)
- Check TX/RX connections are crossed (TX→RX, RX→TX)
- Verify grounds are connected between devices

## Development Notes

- Motor control uses 20kHz PWM frequency for smooth operation
- Speed values are percentage-based (-100 to +100)
- Direction pins use HIGH/LOW for forward/backward
- Firmware designed for real-time motor control with safety features