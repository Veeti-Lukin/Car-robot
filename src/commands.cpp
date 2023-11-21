#include "../inc/commands.h"
#include <nlohmann/json.hpp>

#include <sstream>

const std::string COMMAND_ACTION_KEY = "command_action";
const std::string COMMAND_ARGS_KEY = "command_args";

bool parseJsonCommand(const std::string& json_command, Command& command) {
    try {
        nlohmann::json json = nlohmann::json::parse(json_command);
        command.command_action = json[COMMAND_ACTION_KEY].get<int>();
        if(!getJsonFieldAsType<int>(COMMAND_ACTION_KEY, json, command.command_action)) return false;
        command.command_args = json[COMMAND_ARGS_KEY];

    }
    catch (nlohmann::json::exception&) {
        return false;
    }


    return true;
}
