#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "basic-http-server/inc/http_server/TCPServer.h"
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"

const std::string ROBOT_CONTROL_ROUTE = "robot-api/";

std::string readIndexHtml() {
    std::ifstream file_stream("client_frontend/index.html");
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
};

int main() {
    using http::TcpServer;

    BCM2711_GPIO_DRIVER::initialize();


    TcpServer server("0.0.0.0", 2234);

    // Serve the controlling website
    server.bindHandler(http::types::RequestMethod::GET, "/", [](http::HttpRequest a, http::HttpResponse b) {
        return http::HttpResponse(http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK,
                                  http::types::ContentBodyFormat::text_html, readIndexHtml());
    });

    server.bindHandler(http::types::RequestMethod::GET, "/start?", [](http::HttpRequest a, http::HttpResponse b) {
        std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HANDLER IS WORKING!!! </h1></body></html>";

        http::HttpResponse response(http::types::HttpProtocolVersion::HTTP1_1, http::types::ResponseStatusCode::OK);
        response.setContentBodyFormat(http::types::ContentBodyFormat::text_html);
        response.setContentBody(html);

        return response;
    });
    server.startServing();

    BCM2711_GPIO_DRIVER::terminate();

    return EXIT_SUCCESS;
}