#include "DeviceInfo.hpp"
#include <functional>
#include <iostream>

void ATEMDeviceInfo::dummy_callback(const Message &message) {
    std::cout << "Dummy callback encountered\n";
}

ATEMDeviceInfo::ATEMDeviceInfo(MessageBox * mbox) {
    this->mbox = mbox;
    mbox->registerCallback("_ver", std::bind(&ATEMDeviceInfo::dummy_callback, this, std::placeholders::_1));
};

