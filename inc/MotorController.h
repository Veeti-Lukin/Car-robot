#ifndef MOTORCONTROLLER_HH
#define MOTORCONTROLLER_HH

#include <cstdint>
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/gpio_types.h"

enum MotorSpinDirection{
    forward,
    backwards
};

class L289N_MotorController
{
public:
    L289N_MotorController(uint8_t dir1_pin, uint8_t dir2_pin, PwmPin speed_pin);
    void setSpeed(uint8_t speed_percentage);
    void setDirection(MotorSpinDirection direction);
    void startMotor();
    void stopMotor();
    void setFreq(uint16_t freq);

    /**
     * @brief forceStop to leave this force stop state use setDirection
     */
    void forceStop();
private:
     uint8_t dir1_pin_;
     uint8_t dir2_pin;
     uint8_t speed_pin_;
     uint16_t pwm_freq_ = 20000;
     // used for scaling the speed perentage to the pwm duty cycle so that the speed percentage resolution is not lost
     // for tt motor = 86
     uint8_t effective_ducty_cycle_start = 75;
     bool is_stopped_ = true;
     uint8_t speed_=0;
};

#endif // MOTORCONTROLLER_HH
