#include "features/AudioMixer.hpp"
#include <json.hpp>
#include <iostream>
using json = nlohmann::json;

ATEMAudioMixer::ATEMAudioMixer(MessageBox * mbox) {
    this->mbox = mbox;
    mbox->registerCallback("_AMC", std::bind(&ATEMAudioMixer::handle__AMC, this, std::placeholders::_1));
    mbox->registerCallback("AMIP", std::bind(&ATEMAudioMixer::handle_AMIP, this, std::placeholders::_1));
}

static void hexdump(std::vector<uint8_t> x) { for (auto i: x) { printf("%02X ", i); } printf("\n"); }
ATEMAudioMixerInput::ATEMAudioMixerInput(MessageBox * mbox, std::vector<uint8_t> AMIP_payload) {
    input_index = Message::get_word(AMIP_payload, 8);
    type = (type_t)AMIP_payload[10];
    plugtype = (plugtype_t)AMIP_payload[15];
    status = (status_t)AMIP_payload[16];
    gain = Message::get_word(AMIP_payload, 18);
    pan = Message::get_word(AMIP_payload, 20);
    this->mbox = mbox;
}

void ATEMAudioMixerInput::set_status(status_t new_status) {
    std::vector<uint8_t> arguments = {0x01, // Update status and only that
                                     0x00};// Unknown
    append_word(arguments, input_index);
    arguments.push_back(new_status);
    // fixme: hackish padding
    append_word(arguments, 0x00);
    append_word(arguments, 0x00);
    append_word(arguments, 0x00);
    arguments.push_back(0x00);

    Message inputset = Message::Command("CAMI", arguments);
    auto future = mbox->send_message(inputset);
    future.wait();

}

ATEMAudioMixerInput::type_t ATEMAudioMixerInput::get_type() {
    return this->type;
}

int16_t ATEMAudioMixerInput::get_pan() {
    return this->pan;
}

uint16_t ATEMAudioMixerInput::get_gain() {
    return this->gain;
}

ATEMAudioMixerInput::plugtype_t ATEMAudioMixerInput::get_plugtype() {
    return this->plugtype;
}

ATEMAudioMixerInput::status_t ATEMAudioMixerInput::get_status() {
    return this->status;
}

// Audio Mixer Input
json ATEMAudioMixer::handle_AMIP(const Message &message) {
    uint16_t index = Message::get_word(message.payload, 8);
    inputs[index] = std::make_shared<ATEMAudioMixerInput>(mbox, message.payload);
    json m;
    m["module"] = "AudioMixer";
    m["subject"] = "audio_mixer_input";
    m["input_index"] = index;
    m["input_status"] = inputs[index]->get_status();
    m["input_type"] = inputs[index]->get_type();
    m["input_plugtype"] = inputs[index]->get_plugtype();
    m["gain"] = inputs[index]->get_gain();
    m["pan"] = inputs[index]->get_pan();
    std::cout << m.dump(4);
    return m;
}

// Audio Mixer Configuration
json ATEMAudioMixer::handle__AMC(const Message &message) {
    num_inputs = message.payload[9];
    has_monitor_out = (bool)message.payload[9] & 0x01;

    json m;
    m["module"] = "AudioMixer";
    m["subject"] = "audio_mixer_configuration";
    m["num_channels"] = num_inputs;
    m["has_monitor_out"] = has_monitor_out;
    return m;
}
