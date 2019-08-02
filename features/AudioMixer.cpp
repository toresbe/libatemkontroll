#include "features/AudioMixer.hpp"

ATEMAudioMixer::ATEMAudioMixer(MessageBox * mbox) {
    this->mbox = mbox;
//    mbox->registerCallback("_ver", std::bind(&ATEMAudioMixer::dummy_callback, this, std::placeholders::_1));
//    mbox->registerCallback("_top", std::bind(&ATEMAudioMixer::parse_topology, this, std::placeholders::_1));
};
