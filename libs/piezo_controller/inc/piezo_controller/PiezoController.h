
#ifndef CAR_ROBOT_PIEZOCONTROLLER_H
#define CAR_ROBOT_PIEZOCONTROLLER_H

#include <cstdint>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

#include "software_pwm/SoftwarePwmController.h"

using namespace std::chrono_literals;

class PiezoController {
public:
    struct Sound {
        uint16_t frequency;
        std::chrono::milliseconds duration_ms;
    };

    enum class PlayingProtocol {
        async,
        blocking
    };

    PiezoController(uint8_t pin_num);
    ~PiezoController();

    bool playSound(Sound sound, PlayingProtocol playing_protocol =
            PlayingProtocol::async);
    bool playSoundPattern(std::vector<Sound> sounds,
                          PlayingProtocol playing_protocol = PlayingProtocol::async);
    void stopPlaying();


    void setVolume(uint8_t volume_percentage);

    bool isPlaying() const;

private:
    uint8_t volume_percentage_ = 50;
    bool is_playing_ = false;

    SoftwarePwmController soft_pwm_controller_;

    std::thread work_thread;
    std::mutex mutex_;

    // raising this may cause the playing sound duration to overshoot
    const std::chrono::milliseconds thread_sync_interval_ = 10ms;
};


#endif //CAR_ROBOT_PIEZOCONTROLLER_H
