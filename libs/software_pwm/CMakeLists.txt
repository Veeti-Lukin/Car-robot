add_library(software_pwm
        inc/software_pwm/SoftwarePwmController.h
        src/SoftwarePwmController.cpp
)

target_include_directories(software_pwm PUBLIC inc)

target_link_libraries(software_pwm BCM2711_GPIO_Driver)
