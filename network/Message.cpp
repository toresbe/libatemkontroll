#include "network/Message.hpp"
#include "network/MessageBox.hpp"
#include <iostream>
#include <assert.h>

uint16_t Message::get_word(const std::vector<uint8_t> & input, int index) {
    return input[index+1] | (input[index] << 8);
}

void Message::build_cmd_payload(const std::vector<uint8_t> &arguments) {
    payload.clear();
    unsigned int size = arguments.size() + cmd_name.size() + 4;
    append_word(payload, size);
    append_word(payload, 0);
    assert(cmd_name.size() == 4);
    std::copy(cmd_name.begin(), cmd_name.end(), std::back_inserter(payload));
    payload.insert(payload.end(), arguments.begin(), arguments.end());
}


Message::Message(std::vector<uint8_t> raw_message) {
    if(raw_message.size() < SIZE_OF_HEADER) return;
    this->type = raw_message[0] >> 3;
    auto message_size = get_word(raw_message, 0) & 0x07FF; // need to mask away message type
    //std::cout << "Packet size: " << message_size << ", message size " << raw_message.size() <<"\n";
    (void) message_size; // avoid unused variable warning -- TODO: sanity check
    uid = get_word(raw_message, 2);
    ackid = get_word(raw_message, 4);
    // we skip the next 4 bytes because we don't know what they do
    sequence_num = get_word(raw_message, 10);
    if(raw_message.size() > SIZE_OF_HEADER) {
        assert(raw_message.size() > SIZE_OF_HEADER + 4); // if this fails, time to reassess assumptions
        cmd_name = std::string(raw_message.begin() + 16, raw_message.begin() + 20);
        payload = std::vector<uint8_t>(raw_message.begin() + SIZE_OF_HEADER, raw_message.end());
    }
}

Message::Message(Message::Types packet_type) {
    type = packet_type;
}

Message::Message() {
}

Message Message::Command(std::string cmd_name, std::vector<uint8_t> payload) {
    Message cmd;
    cmd.type = Types::AckReq;
    cmd.cmd_name = cmd_name;
    cmd.build_cmd_payload(payload);
    return cmd;
}

Message Message::ACKFrom(const Message &msg) {
        Message ackmsg(Message::Types::ACK);
        ackmsg.ackid = msg.sequence_num;
        return ackmsg;
}

Message MakeHelloMessage() {
    Message HelloMessage;
    HelloMessage.type = Message::Types::Hello;
    HelloMessage.payload = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    return HelloMessage;
}
