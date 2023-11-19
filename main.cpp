#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "basic-http-server/inc/http_server/TCPServer.h"
//#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"
#include "robot_types.h"

const std::string ROBOT_CONTROL_ROUTE = "/robot-api/command";

std::string readIndexHtml() {
    std::ifstream file_stream("client_frontend/index.html");
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
}

Command parseCommand(const std::string& command_string) {
    Command command;
    command.action = getCommandActionEnum("unknown");

    return command;
}

bool executeCommand(Command command) {
    if (command.action == CommandAction::UNKNOWN) return false;
    switch (command.action) {
        case CommandAction::DRIVE_FORWARD:
            break;
        case CommandAction::DRIVE_BACKWARD:
            break;
        case CommandAction::TURN_LEFT:
            break;
        case CommandAction::TURN_RIGHT:
            break;
        default:
            return false;
    }
    return true;
}

int main() {
    using http::TcpServer;

    //BCM2711_GPIO_DRIVER::initialize();
    //CarController car_controller();


    TcpServer server("0.0.0.0", 2234);

    // Serve the controlling website
    server.bindHandler(http::types::RequestMethod::GET, "/", [](http::HttpRequest a, http::HttpResponse b) {
        return http::HttpResponse(http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK,
                                  http::types::ContentBodyFormat::text_html, readIndexHtml());
    });

    // Bind the robot controlling commands
    server.bindHandler(http::types::RequestMethod::POST, ROBOT_CONTROL_ROUTE, [](http::HttpRequest request, http::HttpResponse response) {
        Command command = parseCommand(request.getContentBody());
        return http::HttpResponse(http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK);
    });

    server.startServing();

    //BCM2711_GPIO_DRIVER::terminate();

    return EXIT_SUCCESS;
}