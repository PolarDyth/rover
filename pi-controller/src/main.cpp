#include "SerialPort.hpp"
#include "ControllerConfig.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void sendCommand(SerialPort& serial, const std::string& command) {
    if (!serial.writeLine(command)) {
        std::cerr << "Failed to send command: " << command << "\n";
        return;
    }

    std::cout << "Sent: " << command << "\n";

    std::string reply = serial.readLine();
    if (!reply.empty()) {
        std::cout << "Received: " << reply << "\n";
    } else {
        std::cout << "No reply received.\n";
    }
}

int main() {
    SerialPort serial(ControllerConfig::SERIAL_DEVICE, ControllerConfig::SERIAL_BAUD_RATE);

    if (!serial.openPort()) {
        std::cerr << "Could not open serial port: " << ControllerConfig::SERIAL_DEVICE << "\n";
        return 1;
    }

    std::cout << "Serial port opened (" << ControllerConfig::SERIAL_DEVICE << "). Waiting for ESP32 to initialize...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::INITIAL_DELAY_MS));

    // Test basic communication
    std::cout << "\n=== Testing Basic Communication ===\n";
    sendCommand(serial, "PING");
    sendCommand(serial, "HELP");

    // Test motor control commands
    std::cout << "\n=== Testing Motor Control ===\n";

    std::cout << "\nMoving forward at 50% speed for 2 seconds...\n";
    sendCommand(serial, "MOVE 0 50");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::MOVEMENT_DURATION_MS));
    sendCommand(serial, "STOP");

    std::cout << "\nMoving backward at 30% speed for 2 seconds...\n";
    sendCommand(serial, "MOVE 2 30");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::MOVEMENT_DURATION_MS));
    sendCommand(serial, "STOP");

    std::cout << "\nTurning left for 1 second...\n";
    sendCommand(serial, "MOVE 3 40");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "STOP");

    std::cout << "\nTurning right for 1 second...\n";
    sendCommand(serial, "MOVE 4 40");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "STOP");

    std::cout << "\nRotating left in place for 1 second...\n";
    sendCommand(serial, "MOVE 5 50");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "STOP");

    std::cout << "\nRotating right in place for 1 second...\n";
    sendCommand(serial, "MOVE 6 50");
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "STOP");

    // Test individual motor control
    std::cout << "\n=== Testing Individual Motor Control ===\n";
    sendCommand(serial, "SET 30,30");  // Both motors forward
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "SET -30,-30"); // Both motors backward
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "SET 30,-30");  // Rotate in place
    std::this_thread::sleep_for(std::chrono::milliseconds(ControllerConfig::COMMAND_DELAY_MS));
    sendCommand(serial, "STOP");

    std::cout << "\n=== Test Complete ===\n";
    return 0;
}
