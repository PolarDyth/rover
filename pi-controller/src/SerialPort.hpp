#pragma once

#include <string>

class SerialPort {
public:
    SerialPort(const std::string& device, int baudRate);
    ~SerialPort();

    bool openPort();
    void closePort();

    bool writeLine(const std::string& text);
    std::string readLine();

    bool isOpen() const;

private:
    std::string device_;
    int baudRate_;
    int fd_;

    bool configurePort();
};
