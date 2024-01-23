
#ifndef CAR_ROBOT_SOFTWAREPWMCONTROLLER_H
#define CAR_ROBOT_SOFTWAREPWMCONTROLLER_H

#include <cstdint>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>

#include "BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"
#include "BCM2711_GPIO_Driver/gpio_types.h"


class SoftwarePwmController {
public:
    explicit SoftwarePwmController(uint8_t pin_num, uint32_t frequency, uint8_t
    duty_cycle);
    ~SoftwarePwmController();

    void configure(uint32_t frequency, uint8_t duty_cycle);

    void enable(bool enable);
private:
    uint8_t pin_num_;
    bool enabled_ = false;
    std::thread* worker_thread_ = nullptr;
    std::chrono::milliseconds on_time_;
    std::chrono::milliseconds off_time_;

    std::mutex mutex_;


    std::function<void()> worker_func = [this](){
        while(enabled_) {
            std::chrono::milliseconds on_time;
            std::chrono::milliseconds off_time;
            {
                std::lock_guard<std::mutex> lock_guard(mutex_);
                on_time = on_time_;
                off_time = off_time_;
            }

            BCM2711_GPIO_DRIVER::setOuputPin(pin_num_, HIGH);
            std::this_thread::sleep_for(on_time);

            BCM2711_GPIO_DRIVER::setOuputPin(pin_num_, LOW);
            std::this_thread::sleep_for(off_time);
        }
    };
};


#endif //CAR_ROBOT_SOFTWAREPWMCONTROLLER_H
