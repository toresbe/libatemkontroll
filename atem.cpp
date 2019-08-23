#include "atem.hpp"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>

ATEM::ATEM() : DeviceInfo(&mbox), MixEffects(&mbox)
{
}

void ATEM::process_events() {
    mbox.process_events();
}

void ATEM::connect(std::string hostname) {
    mbox.connect(hostname);
}
