#pragma once

#include <Arduino.h>

class MotorDriver {
public:
    // Motor configuration structure
    struct MotorConfig {
        uint8_t pwmPin;
        uint8_t dirPin;
        bool invertDirection;
    };

    MotorDriver(const MotorConfig& leftMotor, const MotorConfig& rightMotor);

    // Initialize motor driver
    void begin();

    // Motor control functions
    void setSpeed(int8_t leftSpeed, int8_t rightSpeed);
    void stop();
    void setLeftSpeed(int8_t speed);
    void setRightSpeed(int8_t speed);

    // Movement primitives for rover
    void moveForward(int8_t speed);
    void moveBackward(int8_t speed);
    void turnLeft(int8_t speed);
    void turnRight(int8_t speed);
    void rotateLeft(int8_t speed);
    void rotateRight(int8_t speed);

private:
    MotorConfig leftMotor_;
    MotorConfig rightMotor_;

    // PWM settings
    const int pwmFrequency_ = 20000; // 20kHz for motor control
    const int pwmResolution_ = 8;    // 8-bit resolution (0-255)
    const int pwmChannel_ = 0;       // LED PWM channel

    void setMotorSpeed(const MotorConfig& motor, int8_t speed);
    void configurePWM(uint8_t pin);
};