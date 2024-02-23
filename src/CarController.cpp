#include "../inc/CarController.h"

#include <cmath>
#include <iostream>

#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/gpio_types.h"


CarController::CarController(PwmPin left_motor_speed_pin, uint8_t left_motor_dir1_pin,
                             uint8_t left_motor_dir2_pin, PwmPin right_motor_speed_pin,
                             uint8_t right_motor_dir1_pin, uint8_t right_motor_dir2_pin) :
    right_motor(right_motor_dir1_pin, right_motor_dir2_pin, right_motor_speed_pin),
    left_motor(left_motor_dir1_pin, left_motor_dir2_pin, left_motor_speed_pin)
{
    right_motor.startMotor();
    left_motor.startMotor();
}

void CarController::driveForward()
{
    right_motor.setDirection(forward);
    left_motor.setDirection(forward);
    right_motor.setSpeed(speed_);
    left_motor.setSpeed(speed_);
    std::cout  << speed_ << std::endl;
}

void CarController::driveBackwards()
{
    right_motor.setDirection(backwards);
    left_motor.setDirection(backwards);
    right_motor.setSpeed(speed_);
    left_motor.setSpeed(speed_);
}

void CarController::turnLeft()
{

    right_motor.setDirection(forward);
    left_motor.setDirection(backwards);
    right_motor.setSpeed(speed_);
    left_motor.setSpeed(speed_);
}

void CarController::turnRight()
{
    right_motor.setDirection(backwards);
    left_motor.setDirection(forward);
    right_motor.setSpeed(speed_);
    left_motor.setSpeed(speed_);
}

void CarController::driveRelativeDirection(int8_t angle, int8_t speed)
{
    MotorSpinDirection direction;

    if (speed >= 0 ) {     // driving forward
        direction = MotorSpinDirection::forward;
    }
    else {                // driving backwards
        direction = MotorSpinDirection::backwards;
    }

    speed_  = abs(speed);

    double angle_factor = static_cast<float>(90 - abs(angle)) / 90.0;
    uint8_t inner_motor_speed = static_cast<int>(angle_factor * speed_);

    left_motor.setDirection(direction);
    right_motor.setDirection(direction);

    // driving straight
    if (angle == 0) {
        left_motor.setSpeed(speed_);
        right_motor.setSpeed(speed_);
    }
    // turning to right
    else if(angle > 0) {
        left_motor.setSpeed(speed_);
        right_motor.setSpeed(inner_motor_speed);
    }
    // turing to left
    else {
        left_motor.setSpeed(inner_motor_speed);
        right_motor.setSpeed(speed_);
    }

    std::cout << static_cast<int>(inner_motor_speed) << " - " <<
    static_cast<int>(speed_) << std::endl;
}

void CarController::setSpeed(uint8_t speed_percentage)
{
    speed_ = speed_percentage;
}

void CarController::stop()
{
    left_motor.forceStop();
    right_motor.forceStop();
}

void CarController::setTone(uint16_t tone) {
    tone_ = tone;
    left_motor.setFreq(tone);
    right_motor.setFreq(tone);
}
