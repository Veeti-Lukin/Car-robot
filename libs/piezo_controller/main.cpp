
#include <piezo_controller/PiezoController.h>
#include <iostream>
#include <chrono>


int main() {
    int time;
    uint16_t freq;
    std::string buffer;
    PiezoController piezo_controller(1);

    while (true) {
        std::getline(std::cin, buffer);
        time = std::atoi(buffer.c_str());
        std::getline(std::cin, buffer);
        freq = std::atoi(buffer.c_str());

        piezo_controller.playSound({freq,std::chrono::milliseconds(time)});
    }
};
