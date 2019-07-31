#include "atem.hpp"
#include <iostream>

ATEM::ATEM() : DeviceInfo(&mbox), MixEffects(&mbox)
{
};

static void hexdump(std::vector<uint8_t> x) { for (auto i: x) { printf("%02X ", i); } printf("\n"); }

void ATEM::connect(std::string hostname) {
    mbox.connect(hostname);
    std::cout << "Starting dummy event loop\n";
    while (1){
        mbox.process_events();
    }
}
