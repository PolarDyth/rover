#include <Arduino.h>
#include "MotorDriver.hpp"
#include "CommandProtocol.hpp"
#include "Config.hpp"

// Motor driver instance
MotorDriver motors(
    MotorDriver::MotorConfig{RoverConfig::LEFT_MOTOR_PWM_PIN, RoverConfig::LEFT_MOTOR_DIR_PIN, false},
    MotorDriver::MotorConfig{RoverConfig::RIGHT_MOTOR_PWM_PIN, RoverConfig::RIGHT_MOTOR_DIR_PIN, false}
);

// Buffer for incoming serial data
String serialBuffer;
unsigned long lastCommandTime = 0;

void setup() {
    // Configure UART pins for ESP32-S3 (GPIO 16 = RX, GPIO 18 = TX)
    Serial.begin(RoverConfig::SERIAL_BAUD_RATE);
    Serial.setPins(RoverConfig::UART_RX_PIN, RoverConfig::UART_TX_PIN);
    Serial.setTimeout(RoverConfig::SERIAL_TIMEOUT);

    // Initialize motor driver
    motors.begin();

    // Send ready message
    Serial.println("READY:ESP32 Motor Controller initialized");
    Serial.println("READY:Type HELP for available commands");

    // Initialize command timeout
    lastCommandTime = millis();
}

void loop() {
    // Process incoming serial data
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            // End of command - process it
            if (serialBuffer.length() > 0) {
                processCommand(serialBuffer);
                serialBuffer = "";
                lastCommandTime = millis(); // Reset watchdog timer
            }
        } else if (isprint(c)) {
            // Add printable characters to buffer
            serialBuffer += c;
        }
    }

    // Watchdog: Stop motors if no commands received for timeout period
    if (RoverConfig::ENABLE_WATCHDOG) {
        if (millis() - lastCommandTime > RoverConfig::WATCHDOG_TIMEOUT) {
            motors.stop();
            lastCommandTime = millis(); // Reset to prevent continuous stopping
        }
    }
}

void processCommand(const String& commandStr) {
    // Parse command
    CommandProtocol::Command cmd = CommandProtocol::parseCommand(commandStr);

    // Execute command and get response
    String response = CommandProtocol::executeCommand(cmd, motors);

    // Send response back to Raspberry Pi
    Serial.println(response);
}