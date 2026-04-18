#include "SerialPort.hpp"

#include <iostream>
#include <string>

int main() {
    SerialPort serial("/dev/ttyAMA0", 115200);

    if (!serial.openPort()) {
        std::cerr << "Could not open serial port.\n";
        return 1;
    }

    std::cout << "Serial port opened.\n";

    if (!serial.writeLine("PING")) {
        std::cerr << "Failed to send PING.\n";
        return 1;
    }

    std::cout << "Sent: PING\n";

    std::string reply = serial.readLine();

    if (reply.empty()) {
        std::cout << "No reply received.\n";
    } else {
        std::cout << "Received: " << reply << "\n";
    }

    return 0;
}
