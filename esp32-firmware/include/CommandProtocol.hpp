#pragma once

#include <Arduino.h>
#include <vector>

class CommandProtocol {
public:
    struct Command {
        String action;
        std::vector<int> params;
    };

    // Parse incoming command string
    static Command parseCommand(const String& input);

    // Execute command and return response
    static String executeCommand(const Command& cmd, class MotorDriver& motors);

private:
    // Command handlers
    static String handleMove(const std::vector<int>& params, MotorDriver& motors);
    static String handleStop(const std::vector<int>& params, MotorDriver& motors);
    static String handleSetSpeed(const std::vector<int>& params, MotorDriver& motors);
    static String handlePing(const std::vector<int>& params);
    static String handleStatus(const std::vector<int>& params);
    static String handleHelp(const std::vector<int>& params);
    static String handleUnknown(const String& action);

    // Utility functions
    static std::vector<int> parseParams(const String& paramString);
    static String formatResponse(const String& status, const String& message);
};