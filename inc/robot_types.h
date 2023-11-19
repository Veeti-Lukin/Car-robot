
#ifndef CAR_ROBOT_ROBOT_TYPES_H
#define CAR_ROBOT_ROBOT_TYPES_H

#include <string>

enum class CommandAction {
    DRIVE_FORWARD,
    DRIVE_BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,

    DRIVE_RELATIVE_ANGLE,

    STOP,
    START,

    SET_SPEED,

    UNKNOWN
};

inline CommandAction getCommandActionEnum(const std::string& command_action_string) {
    if (command_action_string == "drive_forward") return CommandAction::DRIVE_FORWARD;
    else if (command_action_string == "drive_backward") return CommandAction::DRIVE_BACKWARD;
    else if (command_action_string == "turn_left") return CommandAction::TURN_LEFT;
    else if (command_action_string == "turn_right") return CommandAction::TURN_RIGHT;
    else if (command_action_string == "drive_relative_angle") return CommandAction::DRIVE_RELATIVE_ANGLE;
    else if (command_action_string == "stop") return CommandAction::STOP;
    else if (command_action_string == "start") return CommandAction::START;
    else if (command_action_string == "set_speed") return CommandAction::SET_SPEED;

    // If the input string doesn't match any known command actions
    return CommandAction::UNKNOWN;
}

struct Command {
    CommandAction action = CommandAction::UNKNOWN;
    // command arguments
    int angle = 0;
    int speed = 0;
    // add more arguments if needed
};

#endif //CAR_ROBOT_ROBOT_TYPES_H
