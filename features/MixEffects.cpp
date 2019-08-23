#include "features/MixEffects.hpp"
#include <assert.h>
#include <iostream>

ATEMMixEffects::ATEMMixEffects(MessageBox * mbox) {
    this->mbox = mbox;
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
