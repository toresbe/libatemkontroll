#include "atem.hpp"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>

ATEM::ATEM() : DeviceInfo(&mbox), MixEffects(&mbox)
{
};

static void hexdump(std::vector<uint8_t> x) { for (auto i: x) { printf("%02X ", i); } printf("\n"); }

void ATEM::connect(std::string hostname) {
    mbox.connect(hostname);
    std::cout << "Starting dummy event loop\n";
    int i = 0;
    while (1){
        i++;
        std::this_thread::sleep_for (std::chrono::seconds(1));
        mbox.process_events();
        std::cout << i << "\n";
        if(i == 10) MixEffects.set_program(1000);
        if(i == 12) MixEffects.set_program(1);
    }
}
