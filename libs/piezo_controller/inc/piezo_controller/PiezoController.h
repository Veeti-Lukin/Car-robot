
#ifndef CAR_ROBOT_PIEZOCONTROLLER_H
#define CAR_ROBOT_PIEZOCONTROLLER_H

#include <cstdint>
#include <thread>

class PiezoController {
public:
    enum class PlayPriority{
        queue,
        override,
    };

    PiezoController(uint8_t pin_num);
    ~PiezoController();

    void asyncPlaySound(uint16_t frequency, int32_t duration_ms, PlayPriority
    play_priority_ = PlayPriority::override);
    void stopPlayingAndFlushQueue();

    void setVolume(uint8_t volume_percentage);

    void flushQueue();
private:
    uint8_t volume_percentage_ = 50;

    std::thread work_thread;
};


#endif //CAR_ROBOT_PIEZOCONTROLLER_H
