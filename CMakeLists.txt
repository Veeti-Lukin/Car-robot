cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)

project(Car-robot
        VERSION 1.0
        DESCRIPTION "Raspberry pi 4b Car robot"
        LANGUAGES CXX)


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

            #inc/PiezoController.h
            #src/PiezoController.cpp
        main.cpp
            )

target_link_libraries(${PROJECT_NAME} PRIVATE
                    basic_http_server
                    BCM2711_GPIO_DRIVER)

target_include_directories(${PROJECT_NAME} PRIVATE inc libs)