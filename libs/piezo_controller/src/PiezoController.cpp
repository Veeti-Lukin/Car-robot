
#include "sound_controller/PiezoController.h"
#include "piezo_controller/PiezoController.h"


PiezoController::PiezoController(uint8_t pin_num) {

}

PiezoController::~PiezoController() {

}

void PiezoController::asyncPlaySound(uint16_t frequency, int32_t duration_ms,
                                PiezoController::PlayPriority play_priority_) {
    work_thread = std::thread();
}

void PiezoController::stopPlayingAndFlushQueue() {

}

void PiezoController::setVolume(uint8_t volume_percentage) {
    volume_percentage_ = volume_percentage;
}

void PiezoController::flushQueue() {

}
