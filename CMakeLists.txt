cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)

project(Car-robot
        VERSION 1.0
        DESCRIPTION "Raspberry pi 4b Car robot"
        LANGUAGES CXX)

include(FetchContent)


add_subdirectory(
        libs
)

#include_directories(
#        src
#        inc
#)

add_executable(${PROJECT_NAME}
            inc/CarController.h
            src/CarController.cpp

            inc/MotorController.h
            src/MotorController.cpp

        inc/commands.h
        src/commands.cpp
        #inc/PiezoController.h
            #src/PiezoController.cpp
        main.cpp
)

FetchContent_Declare(json
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE
                    http_server
                    BCM2711_GPIO_Driver
        nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME} PRIVATE inc libs)

configure_file(client_frontend/index.html ${CMAKE_CURRENT_BINARY_DIR}/client_frontend/index.html COPYONLY)