#include "../inc/MotorController.h"

#include <chrono>
#include <thread>
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"


L289N_MotorController::L289N_MotorController(uint8_t dir1_pin, uint8_t dir2_pin, PwmPin speed_pin):
dir1_pin_(dir1_pin), dir2_pin(dir2_pin), speed_pin_(speed_pin.pin_num){
    BCM2711_GPIO_DRIVER::setPinGpioFunction(speed_pin.pin_num, speed_pin.pwm_func);
    BCM2711_GPIO_DRIVER::setPinGpioFunction(dir1_pin, PinFunction::OUTPUT);
    BCM2711_GPIO_DRIVER::setPinGpioFunction(dir2_pin, PinFunction::OUTPUT);
}


void L289N_MotorController::setSpeed(uint8_t speed_percentage)
{
    if (speed_percentage > 100) speed_percentage = 100;
    // scales the given speed to the effective duty cycle range so that the speed percentage resolution is not lost
    uint8_t duty_cycle_percentage = (speed_percentage != 0) ?
            effective_ducty_cycle_start + (100 - effective_ducty_cycle_start) *
            (speed_percentage / 100.0) : 0;

    if (is_stopped_) {
        // get rid of static friction
        BCM2711_GPIO_DRIVER::configPwmPin(K_PWM_FREQUENCY, 100, speed_pin_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        is_stopped_ = false;
    }

    BCM2711_GPIO_DRIVER::configPwmPin(K_PWM_FREQUENCY, duty_cycle_percentage, speed_pin_);

    if(speed_percentage == 0) is_stopped_ = true;
}

void L289N_MotorController::setDirection(MotorSpinDirection direction)
{
    if(direction == forward) {
       BCM2711_GPIO_DRIVER::setOuputPin(dir1_pin_, HIGH);
       BCM2711_GPIO_DRIVER::setOuputPin(dir2_pin, LOW);
    }
    else{
        BCM2711_GPIO_DRIVER::setOuputPin(dir1_pin_, LOW);
        BCM2711_GPIO_DRIVER::setOuputPin(dir2_pin, HIGH);
    }
}

void L289N_MotorController::startMotor()
{
    BCM2711_GPIO_DRIVER::enablePwmPin(speed_pin_, true);
}

void L289N_MotorController::stopMotor()
{
    is_stopped_ = true;
    BCM2711_GPIO_DRIVER::enablePwmPin(speed_pin_, false);
}

void L289N_MotorController::forceStop()
{
    is_stopped_ = true;
    BCM2711_GPIO_DRIVER::setOuputPin(dir1_pin_, HIGH);
    BCM2711_GPIO_DRIVER::setOuputPin(dir2_pin, HIGH);
}

