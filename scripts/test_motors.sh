#!/bin/bash

# Test script for ESP32 motor controller
# This script sends test commands to the ESP32 via serial port

SERIAL_PORT="/dev/ttyAMA0"
BAUD_RATE=115200

echo "ESP32 Motor Controller Test Script"
echo "===================================="
echo ""

# Check if serial port exists
if [ ! -e "$SERIAL_PORT" ]; then
    echo "Error: Serial port $SERIAL_PORT not found"
    echo "Available serial ports:"
    ls -la /dev/tty* | grep -E "ttyA|ttyU|ttyUSB|ttyACM"
    exit 1
fi

echo "Testing motor controller on $SERIAL_PORT at $BAUD_RATE baud"
echo ""

# Function to send command and display response
send_command() {
    local cmd="$1"
    echo "Sending: $cmd"
    echo "$cmd" > "$SERIAL_PORT"
    sleep 0.5
    response=$(timeout 1 cat "$SERIAL_PORT" 2>/dev/null | head -1)
    if [ ! -z "$response" ]; then
        echo "Response: $response"
    fi
    echo ""
}

# Test basic communication
echo "=== Basic Communication Tests ==="
send_command "PING"
send_command "HELP"

# Test motor control
echo "=== Motor Control Tests ==="
echo "Make sure rover is safe and wheels are off the ground!"
echo "Press Enter to continue or Ctrl+C to abort..."
read

send_command "MOVE 0 50"
sleep 2
send_command "STOP"

send_command "MOVE 2 30"
sleep 2
send_command "STOP"

send_command "MOVE 3 40"
sleep 1
send_command "STOP"

send_command "MOVE 4 40"
sleep 1
send_command "STOP"

send_command "MOVE 5 50"
sleep 1
send_command "STOP"

send_command "MOVE 6 50"
sleep 1
send_command "STOP"

# Test individual motor control
echo "=== Individual Motor Control Tests ==="
send_command "SET 30,30"
sleep 1
send_command "SET -30,-30"
sleep 1
send_command "SET 30,-30"
sleep 1
send_command "STOP"

echo "=== Test Complete ==="