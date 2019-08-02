#include "atem.hpp"
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
        mbox.process_events();
        std::cout << i << "\n";
        if(i == 35) MixEffects.set_program(1000);
        if(i == 37) MixEffects.set_program(1);
    }
}
