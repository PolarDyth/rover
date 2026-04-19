#pragma once

// Pin configuration for ESP32-S3-DEVKIT
// These pins should be adjusted based on your actual wiring to the MDD3A motor driver
namespace RoverConfig {
    // Left motor control pins
    constexpr uint8_t LEFT_MOTOR_PWM_PIN = 4;
    constexpr uint8_t LEFT_MOTOR_DIR_PIN = 5;

    // Right motor control pins
    constexpr uint8_t RIGHT_MOTOR_PWM_PIN = 6;
    constexpr uint8_t RIGHT_MOTOR_DIR_PIN = 7;

    // Serial communication settings
    constexpr long SERIAL_BAUD_RATE = 115200;
    constexpr unsigned long SERIAL_TIMEOUT = 100; // ms

    // UART pin configuration for ESP32-S3
    constexpr int UART_RX_PIN = 16;  // GPIO 16 for RX
    constexpr int UART_TX_PIN = 18;  // GPIO 18 for TX

    // Motor control settings
    constexpr unsigned long COMMAND_TIMEOUT = 500; // ms to wait for complete command
    constexpr int DEFAULT_SPEED = 50;              // Default speed percentage (0-100)
    constexpr int MAX_SPEED = 100;                 // Maximum speed percentage
    constexpr int MIN_SPEED = -100;                // Minimum speed percentage

    // Safety settings
    constexpr unsigned long WATCHDOG_TIMEOUT = 1000; // ms - stop motors if no command received
    constexpr bool ENABLE_WATCHDOG = true;            // Set to false to disable watchdog
};