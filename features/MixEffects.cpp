#include "features/MixEffects.hpp"
#include <assert.h>
#include <iostream>

ATEMMixEffects::ATEMMixEffects(MessageBox * mbox) {
    this->mbox = mbox;
};

std::vector<uint8_t> build_cmd_payload(const std::string &cmd_name, const std::vector<uint8_t> &arguments) {
    std::vector<uint8_t> cmd_payload;
    unsigned int size = arguments.size() + cmd_name.size() + 4;
    append_word(cmd_payload, size);
    append_word(cmd_payload, 0);
    assert(cmd_name.size() == 4);
    std::copy(cmd_name.begin(), cmd_name.end(), std::back_inserter(cmd_payload));
    cmd_payload.insert(cmd_payload.end(), arguments.begin(), arguments.end());
    return cmd_payload;
}

void ATEMMixEffects::set_program(uint16_t input_index) {
    Message inputset;
    inputset.type = Message::Types::AckReq;
    inputset.cmd_name = "CPgI";
    std::vector<uint8_t> arguments = {0x00, // ME index (FIXME: should not be magic)
                                     0x00};// Unknown
    append_word(arguments, input_index);
    inputset.payload = build_cmd_payload("CPgI", arguments);
    auto future = mbox->send_message(inputset);
    future.wait();
};
