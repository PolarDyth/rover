#pragma once

#include <string>

namespace ControllerConfig {
    // Serial communication settings
    const std::string SERIAL_DEVICE = "/dev/ttyAMA0"; // Adjust based on your Pi UART configuration
    const int SERIAL_BAUD_RATE = 115200;

    // Timing settings
    const int INITIAL_DELAY_MS = 2000;     // Delay for ESP32 initialization
    const int COMMAND_DELAY_MS = 1000;     // Delay between test commands
    const int MOVEMENT_DURATION_MS = 2000; // Duration for movement commands
};