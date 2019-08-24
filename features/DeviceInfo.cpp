#include "DeviceInfo.hpp"
#include <functional>
#include <iostream>
#include <assert.h>
#include "loguru.hpp"

#include <json.hpp>
using json = nlohmann::json;

ATEMDeviceInfo::ATEMDeviceInfo(MessageBox * mbox) {
    this->mbox = mbox;
    mbox->registerCallback("_top", std::bind(&ATEMDeviceInfo::parse_topology, this, std::placeholders::_1));
    mbox->registerCallback("Time", std::bind(&ATEMDeviceInfo::handle_state_timecode, this, std::placeholders::_1));
}

json ATEMDeviceInfo::handle_state_timecode(const Message &message) {
    json state_change;
    char timecode[13];
    snprintf(timecode, sizeof(timecode), "%02d:%02d:%02d.%02d",
            message.payload[8], message.payload[9],
            message.payload[10], message.payload[11]);

    state_change["module"] = "device_info";
    state_change["timecode"] = timecode;
    state_change["subject"] = "last_state_change_timecode";
    return(state_change);
}

json ATEMDeviceInfo::parse_topology(const Message &message) {
    json state_update;

    num_MEs = message.payload[8];
    num_srcs = message.payload[9];
    num_colorgens = message.payload[10];
    num_auxes = message.payload[11];
    num_DSKs = message.payload[12];
    num_stingers = message.payload[13];
    num_DVEs = message.payload[14];
    num_supersrcs = message.payload[15];
    has_SD_output = message.payload[16];

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

    return(state_update);
}
