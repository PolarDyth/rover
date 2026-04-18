
#include "SerialPort.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

SerialPort::SerialPort(const std::string& device, int baudRate)
    : device_(device), baudRate_(baudRate), fd_(-1) {}

SerialPort::~SerialPort() {
    closePort();
}

bool SerialPort::openPort() {
    fd_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_ < 0) {
        std::cerr << "Failed to open serial port " << device_
                  << ": " << std::strerror(errno) << "\n";
        return false;
    }

    return configurePort();
}

void SerialPort::closePort() {
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}

bool SerialPort::isOpen() const {
    return fd_ >= 0;
}

bool SerialPort::configurePort() {
    termios tty{};

    if (tcgetattr(fd_, &tty) != 0) {
        std::cerr << "tcgetattr failed: " << std::strerror(errno) << "\n";
        return false;
    }

    // Set baud rate
    speed_t speed;
    switch (baudRate_) {
        case 9600:   speed = B9600; break;
        case 19200:  speed = B19200; break;
        case 38400:  speed = B38400; break;
        case 57600:  speed = B57600; break;
        case 115200: speed = B115200; break;
        default:
            std::cerr << "Unsupported baud rate: " << baudRate_ << "\n";
            return false;
    }

    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);

    // 8N1, no flow control, raw mode
    tty.c_cflag &= ~PARENB;            // no parity
    tty.c_cflag &= ~CSTOPB;            // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                // 8 bits
    tty.c_cflag &= ~CRTSCTS;           // no hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;     // turn on READ, ignore modem ctrl lines

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // no software flow control
    tty.c_iflag &= ~(INLCR | ICRNL | IGNCR);        // don't translate newlines
    tty.c_oflag &= ~OPOST;                          // raw output

    // Read timeout behavior
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10; // 1.0 second timeout

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        std::cerr << "tcsetattr failed: " << std::strerror(errno) << "\n";
        return false;
    }

    return true;
}

bool SerialPort::writeLine(const std::string& text) {
    if (fd_ < 0) {
        return false;
    }

    std::string message = text + "\n";
    ssize_t bytesWritten = write(fd_, message.c_str(), message.size());

    return bytesWritten == static_cast<ssize_t>(message.size());
}

std::string SerialPort::readLine() {
    if (fd_ < 0) {
        return "";
    }

    std::string result;
    char ch;

    while (true) {
        ssize_t n = read(fd_, &ch, 1);

        if (n > 0) {
            if (ch == '\n') {
                break;
            }
            if (ch != '\r') {
                result += ch;
            }
        } else {
            // timeout or error
            break;
        }
    }

    return result;
}
