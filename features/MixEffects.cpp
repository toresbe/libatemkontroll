#include "features/MixEffects.hpp"
#include <assert.h>
#include <iostream>

#include <json.hpp>
using json = nlohmann::json;

ATEMMixEffects::ATEMMixEffects(MessageBox * mbox) {
    this->mbox = mbox;
    mbox->registerCallback("PrgI", std::bind(&ATEMMixEffects::handle_PrgI, this, std::placeholders::_1));
    mbox->registerCallback("PrvI", std::bind(&ATEMMixEffects::handle_PrvI, this, std::placeholders::_1));
    mbox->registerCallback("TrPs", std::bind(&ATEMMixEffects::handle_TrPs, this, std::placeholders::_1));
}

json ATEMMixEffects::handle_PrvI(const Message &message) {
    json new_preview;

    auto input_index = Message::get_word(message.payload, 10);
    auto me_index = message.payload[8];
    state[me_index].preview_input = input_index;

    new_preview["module"] = "ME";
    new_preview["subject"] = "preview_input_changed";
    new_preview["index"] = me_index;
    new_preview["input_index"] = input_index;

    return new_preview;
}

json ATEMMixEffects::handle_PrgI(const Message &message) {
    json new_program;

    auto input_index = Message::get_word(message.payload, 10);
    auto me_index = message.payload[8];
    state[me_index].program_input = input_index;

    new_program["module"] = "ME";
    new_program["subject"] = "program_input_changed";
    new_program["index"] = me_index;
    new_program["input_index"] = input_index;

    return new_program;
}

json ATEMMixEffects::handle_TrPs(const Message &message) {
    json new_transition_position;
    new_transition_position["module"] = "ME";
    new_transition_position["subject"] = "new_transition_position";
    new_transition_position["index"] = message.payload[8];
    new_transition_position["active"] = message.payload[9];
    new_transition_position["frames_remaining"] = message.payload[10];
    new_transition_position["position"] = Message::get_word(message.payload, 12);
    return new_transition_position;
}

void ATEMMixEffects::set_preview(uint16_t input_index) {
    std::vector<uint8_t> arguments = {me_index,
                                     0x00};// Unknown
    append_word(arguments, input_index);
    Message inputset = Message::Command("CPvI", arguments);
    auto future = mbox->send_message(inputset);
    future.wait();
}

void ATEMMixEffects::take_cut() {
    Message cutmsg = Message::Command("DCut", {me_index, 0x00, 0x00, 0x00});
    auto future = mbox->send_message(cutmsg);
    future.wait();
}

void ATEMMixEffects::take_auto() {
    Message automsg = Message::Command("DAut", {me_index, 0x00, 0x00, 0x00});
    auto future = mbox->send_message(automsg);
    future.wait();
}

void ATEMMixEffects::set_program(uint16_t input_index) {
    std::vector<uint8_t> arguments = {me_index,
                                     0x00};// Unknown
    append_word(arguments, input_index);
    Message inputset = Message::Command("CPgI", arguments);
    auto future = mbox->send_message(inputset);
    future.wait();
}
