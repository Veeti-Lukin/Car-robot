
#include "software_pwm/SoftwarePwmController.h"

SoftwarePwmController::SoftwarePwmController(uint8_t pin_num, uint32_t frequency, uint8_t
duty_cycle) {
    BCM2711_GPIO_DRIVER::setPinGpioFunction(pin_num, PinFunction::OUTPUT);
    BCM2711_GPIO_DRIVER::setOuputPin(pin_num_, LOW);

    configure(frequency, duty_cycle);
}

SoftwarePwmController::~SoftwarePwmController() {
    enable(false);
}

void SoftwarePwmController::enable(bool enable) {
    // nothing should be done if <enable> status is the same than already set
    // would lead to memory leak or nullpointer accessing when thread is deleted
    if(enable == enabled_) return;
    enabled_ = enable;

    if(enabled_) {
        worker_thread_ = new std::thread(worker_func);
    } else {
        if(worker_thread_) worker_thread_->join();
        delete worker_thread_;
        worker_thread_ = nullptr;
        BCM2711_GPIO_DRIVER::setOuputPin(pin_num_, LOW);
    }
}

void SoftwarePwmController::configure(uint32_t frequency, uint8_t duty_cycle) {
    if (duty_cycle > 100) duty_cycle = 100;

    on_time_ = std::chrono::milliseconds((1000-(1000/(frequency*1000))) * duty_cycle);
    off_time_ = std::chrono::milliseconds(1000) - on_time_;
}
