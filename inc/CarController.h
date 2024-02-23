#ifndef CARCONTROLLER_HH
#define CARCONTROLLER_HH

#include <cstdint>
#include "MotorController.h"
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/gpio_types.h"


class CarController
{
public:
    CarController(PwmPin left_motor_speed_pin, uint8_t left_motor_dir1_pin,
                  uint8_t left_motor_dir2_pin, PwmPin right_motor_speed_pin,
                  uint8_t right_motor_dir1_pin, uint8_t right_motor_dir2_pin);

    void driveForward();
    void driveBackwards();
    void turnLeft();
    void turnRight();

    void setSpeed(uint8_t speed_percentage);

    // angle from 90 to -90 and speed 100 to -100
    void driveRelativeDirection(int8_t angle, int8_t speed);

    void stop();

    void setTone(uint16_t tone);

private:

    L289N_MotorController right_motor;
    L289N_MotorController left_motor;

    uint8_t speed_ = 0;
    uint16_t tone_ = 40000;
};

#endif // CARCONTROLLER_HH
