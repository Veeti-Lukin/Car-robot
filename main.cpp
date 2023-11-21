#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "basic-http-server/inc/http_server/TCPServer.h"
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"
#include "commands.h"
#include "CarController.h"

const std::string ROBOT_MOVEMENT_COMMAND_ROUTE = "/robot-api/movement-command";

std::string readIndexHtml() {
    std::ifstream file_stream("client_frontend/index.html");
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
}

http::HttpResponse createFailedCommandResponse() {
    return {http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::BadRequest};
}

http::HttpResponse createExecutedCommandResponse() {
    return {http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK};
}



bool executeMovementCommand(const Command& command, CarController car_controller) {
    switch ((MoveCommandAction)command.command_action) {
        case MoveCommandAction::DRIVE_FORWARD:
            car_controller.driveForward();
            return true;
        case MoveCommandAction::DRIVE_BACKWARD:
           car_controller.driveBackwards();
            return true;
        case MoveCommandAction::TURN_LEFT:
            car_controller.turnLeft();
            return true;
        case MoveCommandAction::TURN_RIGHT:
            car_controller.turnRight();
            return true;
        case MoveCommandAction::DRIVE_RELATIVE_ANGLE:
            break;
        case MoveCommandAction::STOP_MOTORS:
            break;
        case MoveCommandAction::START_MOTORS:
            break;
        case MoveCommandAction::SET_SPEED:
            break;
        case MoveCommandAction::UNKNOWN:
        default:
            break;
    }
    return false;
}


int main() {
    using http::TcpServer;

    BCM2711_GPIO_DRIVER::initialize();
    CarController car_controller(K_PWM_PIN12, 7, 1, K_PWM_PIN18, 15, 14);


    TcpServer server("0.0.0.0", 2234);

    // Serve the controlling website
    server.bindHandler(http::types::RequestMethod::GET, "/", [](http::HttpRequest a, http::HttpResponse b) {
        return http::HttpResponse(http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK,
                                  http::types::ContentBodyFormat::text_html, readIndexHtml());
    });

    // Bind the robot controlling commands
    server.bindHandler(http::types::RequestMethod::POST, ROBOT_MOVEMENT_COMMAND_ROUTE, [&](http::HttpRequest request, http::HttpResponse response) {
        Command command;
        if(!parseJsonCommand(request.getContentBody(), command)) {
            return createFailedCommandResponse();
        }

        if (!executeMovementCommand(command, car_controller)) {
            return createFailedCommandResponse();
        }
        return createExecutedCommandResponse();
    });

    server.startServing();

    BCM2711_GPIO_DRIVER::terminate();

    return EXIT_SUCCESS;
}