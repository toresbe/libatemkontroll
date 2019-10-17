#include "atem.hpp"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>

ATEM::ATEM() : DeviceInfo(&mbox), MixEffects(&mbox), AudioMixer(&mbox)
{
}

std::string ATEM::process_events() {
    return mbox.process_events();
}

void ATEM::connect(std::string hostname) {
    mbox.connect(hostname);
}
