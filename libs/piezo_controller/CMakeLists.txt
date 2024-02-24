add_library(piezo_controller
        inc/piezo_controller/PiezoController.h
        src/PiezoController.cpp

        inc/piezo_controller/notes.h
)

target_include_directories(piezo_controller PUBLIC inc)

target_link_libraries(piezo_controller software_pwm)



add_executable(piezo_controller_test main.cpp)
target_link_libraries(piezo_controller_test PUBLIC piezo_controller)
