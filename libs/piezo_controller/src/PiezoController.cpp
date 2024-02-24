
#include "piezo_controller/PiezoController.h"

PiezoController::PiezoController(uint8_t pin_num):
soft_pwm_controller_(pin_num, 0, 0) {

}

PiezoController::~PiezoController() {
    stopPlaying();
    if(work_thread.joinable()) {
        work_thread.join();
    }
}

bool PiezoController::playSound(PiezoController::Sound sound,
                                PiezoController::PlayingProtocol playing_protocol) {
    // sound should not be played if there is already a sound playing
    {
        std::lock_guard<std::mutex> lock_guard(mutex_);
        if(is_playing_) return false;
    }


    work_thread = std::thread([&]{
        uint8_t volume_percentage;
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            volume_percentage = volume_percentage_;
            is_playing_ = true;
        }

        // greatest resonance(aka most volume) is achieved at 50% duty cycle
        // going under or over than that just lowers the volume
        uint8_t duty_cycle = volume_percentage/2;

        soft_pwm_controller_.configure(sound.frequency, duty_cycle);
        soft_pwm_controller_.enable(true);

        std::chrono::time_point<std::chrono::system_clock> timeout_time_point =
                std::chrono::system_clock::now() + sound.duration_ms;

        // this loop runs until the duration given to <sound> is completed
        // or if sound playing is interrupted with stopPlaying().
        // This loop also syncs the volume if it is modified while the
        // sound is playing
        while (std::chrono::system_clock::now() < timeout_time_point) {
            std::this_thread::sleep_for(thread_sync_interval_);
            {
                std::lock_guard<std::mutex> lock_guard(mutex_);
                // playing sound has been disabled with <stopPlaying>
                if(!is_playing_) break;

                if(volume_percentage_ != volume_percentage) {
                    volume_percentage = volume_percentage_;
                    duty_cycle = volume_percentage/2;
                    soft_pwm_controller_.configure(sound.frequency, duty_cycle);
                }
            }
        }
        soft_pwm_controller_.enable(false);
                });

    if(playing_protocol == PlayingProtocol::blocking) {
        work_thread.join();
    }

    return true;
}

bool PiezoController::playSoundPattern(std::vector<Sound> sounds,
                                       PiezoController::PlayingProtocol playing_protocol) {
    // sound should not be played if there is already a sound playing
    {
        std::lock_guard<std::mutex> lock_guard(mutex_);
        if(is_playing_) return false;
    }

    work_thread = std::thread([&]{
        uint8_t volume_percentage;
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            volume_percentage = volume_percentage_;
            is_playing_ = true;
        }

        // greatest resonance(aka most volume) is achieved at 50% duty cycle
        // going under or lower than that achieves just lowers the volume
        uint8_t duty_cycle = volume_percentage/2;

        // iterate over all the sounds
        bool should_stop = false;
        for(Sound sound : sounds) {
            if(should_stop) break;

            soft_pwm_controller_.configure(sound.frequency, duty_cycle);
            soft_pwm_controller_.enable(true);

            std::chrono::time_point<std::chrono::system_clock> timeout_time_point =
                                                                       std::chrono::system_clock::now() +
                                                                       sound.duration_ms;

            // this loop runs until the duration given to <sound> is completed
            // or if sound playing is interrupted with stopPlaying().
            // This loop also syncs the volume if it is modified while the
            // sound is playing
            while (std::chrono::system_clock::now() < timeout_time_point) {
                std::this_thread::sleep_for(thread_sync_interval_);
                {
                    std::lock_guard<std::mutex> lock_guard(mutex_);
                    if (!is_playing_) {
                        should_stop = true;
                        break;
                    }

                    if (volume_percentage_ != volume_percentage) {
                        volume_percentage = volume_percentage_;
                        duty_cycle = volume_percentage / 2;
                        soft_pwm_controller_.configure(sound.frequency, duty_cycle);
                    }
                }
            }
        }

        soft_pwm_controller_.enable(false);
    });

    if(playing_protocol == PlayingProtocol::blocking) {
        work_thread.join();
    }

    return true;
}

void PiezoController::stopPlaying() {
    is_playing_ = false;
}


void PiezoController::setVolume(uint8_t volume_percentage) {
    if(volume_percentage > 100) volume_percentage = 100;

    volume_percentage_ = volume_percentage;
}

bool PiezoController::isPlaying() const {
    return is_playing_;
}
