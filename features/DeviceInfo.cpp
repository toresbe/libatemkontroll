#include "DeviceInfo.hpp"
#include <nlohmann/json.hpp>
#include <functional>
#include <iostream>
#include <assert.h>

using json = nlohmann::json;

void ATEMDeviceInfo::dummy_callback(const Message &message) {
    (void)message; // avoid unused var message
    std::cout << "Dummy callback encountered\n";
}

ATEMDeviceInfo::ATEMDeviceInfo(MessageBox * mbox) {
    this->mbox = mbox;
    mbox->registerCallback("_ver", std::bind(&ATEMDeviceInfo::dummy_callback, this, std::placeholders::_1));
    mbox->registerCallback("_top", std::bind(&ATEMDeviceInfo::parse_topology, this, std::placeholders::_1));
}

void ATEMDeviceInfo::parse_topology(const Message &message) {
    assert(message.payload.size() == 12);
    json state_update;

    num_MEs = message.payload[0];
    num_srcs = message.payload[1];
    num_colorgens = message.payload[2];
    num_auxes = message.payload[3];
    num_DSKs = message.payload[4];
    num_stingers = message.payload[5];
    num_DVEs = message.payload[6];
    num_supersrcs = message.payload[7];
    has_SD_output = message.payload[9];

    state_update["subject"] = "topology";
    state_update["num_MEs"] = num_MEs;
    state_update["num_srcs"] = num_srcs;
    state_update["num_colorgens"] = num_colorgens;
    state_update["num_auxes"] = num_auxes;
    state_update["num_DSKs"] = num_DSKs;
    state_update["num_stingers"] = num_stingers;
    state_update["num_DVEs"] = num_DVEs;
    state_update["num_supersrcs"] = num_supersrcs;
    state_update["has_SD_output"] = has_SD_output;
}
