#include "MotorDriver.hpp"

MotorDriver::MotorDriver(const MotorConfig& leftMotor, const MotorConfig& rightMotor)
    : leftMotor_(leftMotor), rightMotor_(rightMotor) {}

void MotorDriver::begin() {
    // Configure direction pins as outputs
    pinMode(leftMotor_.dirPin, OUTPUT);
    pinMode(rightMotor_.dirPin, OUTPUT);

    // Configure PWM for motor speed control
    configurePWM(leftMotor_.pwmPin);
    configurePWM(rightMotor_.pwmPin);

    // Initialize motors to stopped state
    stop();
}

void MotorDriver::setSpeed(int8_t leftSpeed, int8_t rightSpeed) {
    setMotorSpeed(leftMotor_, leftSpeed);
    setMotorSpeed(rightMotor_, rightSpeed);
}

void MotorDriver::stop() {
    setSpeed(0, 0);
}

void MotorDriver::setLeftSpeed(int8_t speed) {
    setMotorSpeed(leftMotor_, speed);
}

void MotorDriver::setRightSpeed(int8_t speed) {
    setMotorSpeed(rightMotor_, speed);
}

void MotorDriver::moveForward(int8_t speed) {
    setSpeed(speed, speed);
}

void MotorDriver::moveBackward(int8_t speed) {
    setSpeed(-speed, -speed);
}

void MotorDriver::turnLeft(int8_t speed) {
    // Turn left by reducing left motor speed
    setSpeed(speed / 2, speed);
}

void MotorDriver::turnRight(int8_t speed) {
    // Turn right by reducing right motor speed
    setSpeed(speed, speed / 2);
}

void MotorDriver::rotateLeft(int8_t speed) {
    // Rotate in place by running motors in opposite directions
    setSpeed(-speed, speed);
}

void MotorDriver::rotateRight(int8_t speed) {
    // Rotate in place by running motors in opposite directions
    setSpeed(speed, -speed);
}

void MotorDriver::setMotorSpeed(const MotorConfig& motor, int8_t speed) {
    // Clamp speed to valid range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;

    // Set direction
    bool direction = speed >= 0;
    if (motor.invertDirection) {
        direction = !direction;
    }
    digitalWrite(motor.dirPin, direction ? HIGH : LOW);

    // Set PWM duty cycle (convert -100..100 to 0..255)
    uint8_t pwmValue = abs(speed) * 255 / 100;
    ledcWrite(pwmChannel_, pwmValue);
}

void MotorDriver::configurePWM(uint8_t pin) {
    // Configure LED PWM for motor control
    ledcSetup(pwmChannel_, pwmFrequency_, pwmResolution_);
    ledcAttachPin(pin, pwmChannel_);
    ledcWrite(pwmChannel_, 0); // Start with 0 speed
}