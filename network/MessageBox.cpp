#include "MessageBox.hpp"
#include <iostream>
#include <arpa/inet.h> // for htons()

static void hexdump(std::vector<uint8_t> x) { for (auto i: x) { printf("%02X ", i); } printf("\n"); }

void MessageBox::process_events() {
    auto incoming = socket.recv();
    hexdump(incoming);
    auto msg = Message(incoming);
    std::cout << "msg has uid " << msg.uid << " ackid " << msg.ackid << " type " << msg.type << " seq_num " << msg.sequence_num << "\n";
    // Apparently this is the only way to tell if the initial state dump is over.
    // After this, the switcher will begin expecting acknowledgements of every
    // packet.
    if(!(this->is_initialized | msg.payload.size())) {
        std::cout << "Now we are initialized!\n";
        is_initialized = true;
    }
    if(msg.type & Message::Types::AckReq) {
        if(is_initialized) {
            *this << Message::ACKFrom(msg);
        }
        if(msg.payload.size()) {
            auto callback = callback_map.find(msg.cmd_name);
            if(callback == callback_map.end()) {
                std::cout << "Received unhandled \"" << msg.cmd_name << "\" packet\n";
            } else {
                std::cout << "Received handled \"" << msg.cmd_name << "\" packet\n";
                callback->second(msg);
            }
        }
    }
    if(msg.type & Message::Types::Hello) {
        std::cout << "Received new hello packet\n";
        is_initialized = false;
        *this << Message(Message::Types::ACK);
    }
    // At this stage, after the relevant parsing callback has been invoked
    // to update the internal state of the class, we need to see if any get
    // or set functions have requested the results, and if so, trigger the
    // relevant callback or whatever
    current_UID = msg.uid;
}

MessageBox::MessageBox() {
}

void MessageBox::registerCallback(const std::string & command_name, const callback_t & callback_function) {
    if(callback_map.find(command_name) != callback_map.end()) {
        std::cout << "Warning: Re-registering callback \"" << command_name << "\"!\n";
    } else {
        std::cout << "Rgistering callback \"" << command_name << "\"!\n";
    }
    callback_map[command_name] = callback_function;
}

void MessageBox::connect(std::string hostname) {
    socket.open(hostname);
    auto hello = MakeHelloMessage();
    *this << hello;
};

void MessageBox::operator<< (const Message & msg) {
    std::vector<uint8_t> raw_message = serialize(msg);
    socket.send(raw_message);
};

void append_word(std::vector<uint8_t> & vector, uint16_t value) {
    vector.push_back((uint8_t)(value >> 8 & 0xFF));
    vector.push_back((uint8_t)(value & 0xFF));
}

// FIXME: This function is duplicated
static uint16_t get_word(const std::vector<uint8_t> & input, int index) {
    return input[index] | (input[index+1] << 16);
};

std::vector<uint8_t> MessageBox::serialize(const Message & msg) {
    std::vector<uint8_t> raw_message;
    uint16_t package_ID = 0;

    // Hello and ACK packets do not increment the packet counter nor do they include it
    if(!(msg.type & (Message::Types::Hello | Message::Types::ACK))) package_ID = ++packet_seq_id;

    // Message type and n_bytes are combined in the first word of the message
    append_word(raw_message, (msg.type << 11) | (msg.payload.size() + SIZE_OF_HEADER));
    append_word(raw_message, current_UID);
    append_word(raw_message, msg.ackid); // ACK_ID ; dunno what this is yet
    append_word(raw_message, msg.sequence_num); // PackageID
    append_word(raw_message, 0); // Padding word
    //append_word(raw_message, 0); // Padding word
    append_word(raw_message, package_ID); // PackageID
    raw_message.insert(raw_message.end(), msg.payload.begin(), msg.payload.end());

    hexdump(raw_message);
    return raw_message;
};
