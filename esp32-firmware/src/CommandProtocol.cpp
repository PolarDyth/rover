#include "CommandProtocol.hpp"
#include "MotorDriver.hpp"

CommandProtocol::Command CommandProtocol::parseCommand(const String& input) {
    Command cmd;
    String trimmed = input;
    trimmed.trim();

    // Find first space to separate action from parameters
    int spaceIndex = trimmed.indexOf(' ');
    if (spaceIndex == -1) {
        cmd.action = trimmed;
    } else {
        cmd.action = trimmed.substring(0, spaceIndex);
        String paramString = trimmed.substring(spaceIndex + 1);
        cmd.params = parseParams(paramString);
    }

    cmd.action.toUpperCase();
    return cmd;
}

String CommandProtocol::executeCommand(const Command& cmd, MotorDriver& motors) {
    if (cmd.action == "MOVE" || cmd.action == "M") {
        return handleMove(cmd.params, motors);
    } else if (cmd.action == "STOP" || cmd.action == "S") {
        return handleStop(cmd.params, motors);
    } else if (cmd.action == "SET" || cmd.action == "SET_SPEED") {
        return handleSetSpeed(cmd.params, motors);
    } else if (cmd.action == "PING") {
        return handlePing(cmd.params);
    } else if (cmd.action == "STATUS") {
        return handleStatus(cmd.params);
    } else if (cmd.action == "HELP") {
        return handleHelp(cmd.params);
    } else {
        return handleUnknown(cmd.action);
    }
}

String CommandProtocol::handleMove(const std::vector<int>& params, MotorDriver& motors) {
    if (params.size() < 1) {
        return formatResponse("ERROR", "MOVE requires direction parameter");
    }

    String direction;
    int8_t speed = 50; // default speed

    if (params.size() >= 2) {
        speed = constrain(params[1], -100, 100);
    }

    switch (params[0]) {
        case 0: // Forward
        case 1: // Forward (alternative)
            motors.moveForward(speed);
            direction = "forward";
            break;
        case 2: // Backward
            motors.moveBackward(speed);
            direction = "backward";
            break;
        case 3: // Left
            motors.turnLeft(speed);
            direction = "left";
            break;
        case 4: // Right
            motors.turnRight(speed);
            direction = "right";
            break;
        case 5: // Rotate Left
            motors.rotateLeft(speed);
            direction = "rotate_left";
            break;
        case 6: // Rotate Right
            motors.rotateRight(speed);
            direction = "rotate_right";
            break;
        default:
            return formatResponse("ERROR", "Invalid direction. Use: 0=forward, 1=forward, 2=backward, 3=left, 4=right, 5=rotate_left, 6=rotate_right");
    }

    return formatResponse("OK", "Moving " + direction + " at speed " + String(speed));
}

String CommandProtocol::handleStop(const std::vector<int>& params, MotorDriver& motors) {
    motors.stop();
    return formatResponse("OK", "Motors stopped");
}

String CommandProtocol::handleSetSpeed(const std::vector<int>& params, MotorDriver& motors) {
    if (params.size() < 2) {
        return formatResponse("ERROR", "SET requires left_speed and right_speed parameters");
    }

    int8_t leftSpeed = constrain(params[0], -100, 100);
    int8_t rightSpeed = constrain(params[1], -100, 100);

    motors.setSpeed(leftSpeed, rightSpeed);
    return formatResponse("OK", "Speed set: L=" + String(leftSpeed) + " R=" + String(rightSpeed));
}

String CommandProtocol::handlePing(const std::vector<int>& params) {
    return formatResponse("PONG", "ESP32 Motor Controller");
}

String CommandProtocol::handleStatus(const std::vector<int>& params) {
    return formatResponse("OK", "Motor controller ready");
}

String CommandProtocol::handleHelp(const std::vector<int>& params) {
    String help = "Available commands:\n";
    help += "  MOVE <direction> [speed] - Move rover (0=forward, 2=backward, 3=left, 4=right, 5=rot_left, 6=rot_right)\n";
    help += "  STOP - Stop all motors\n";
    help += "  SET <left_speed> <right_speed> - Set individual motor speeds (-100 to 100)\n";
    help += "  PING - Ping the motor controller\n";
    help += "  STATUS - Get controller status\n";
    help += "  HELP - Show this help message";
    return formatResponse("OK", help);
}

String CommandProtocol::handleUnknown(const String& action) {
    return formatResponse("ERROR", "Unknown command: " + action + ". Type HELP for available commands.");
}

std::vector<int> CommandProtocol::parseParams(const String& paramString) {
    std::vector<int> params;
    int start = 0;
    int comma = paramString.indexOf(',');

    while (comma != -1) {
        String param = paramString.substring(start, comma);
        param.trim();
        if (param.length() > 0) {
            params.push_back(param.toInt());
        }
        start = comma + 1;
        comma = paramString.indexOf(',', start);
    }

    // Add last parameter
    String lastParam = paramString.substring(start);
    lastParam.trim();
    if (lastParam.length() > 0) {
        params.push_back(lastParam.toInt());
    }

    return params;
}

String CommandProtocol::formatResponse(const String& status, const String& message) {
    return status + ":" + message;
}