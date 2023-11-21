
#ifndef CAR_ROBOT_COMMANDS_H
#define CAR_ROBOT_COMMANDS_H

#include <string>
#include <climits>
#include <unordered_map>
#include <nlohmann/json.hpp>

enum class MoveCommandAction : int {
    DRIVE_FORWARD = 0,
    DRIVE_BACKWARD = 1,
    TURN_LEFT = 2,
    TURN_RIGHT =  3,
    DRIVE_RELATIVE_ANGLE = 4,
    STOP_MOTORS = 5,
    START_MOTORS = 6,
    SET_SPEED = 7,

    UNKNOWN = INT_MAX
};

struct Command{
    int command_action;
    nlohmann::json command_args;
};

template <typename T>
bool getJsonFieldAsType(const std::string& key, const nlohmann::json& json, T& target){
    try {
        target = json[key].get<T>();
    } catch (nlohmann::json::exception&) {
        return false;
    }
    return true;
}

bool parseJsonCommand(const std::string& json_command, Command& command);

#endif //CAR_ROBOT_COMMANDS_H
