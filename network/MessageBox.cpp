#include "MessageBox.hpp"
#include <future>
#include <mutex>
#include <memory>
#include <iostream>
#include <arpa/inet.h> // for htons()

static void hexdump(std::vector<uint8_t> x) { for (auto i: x) { printf("%02X ", i); } printf("\n"); }

// TODO: Implement retransmit. Currently, if a packet is lost,
// we'll just give up.
void timeout_function(int packet_seq_id) {
}

std::future<void> MessageBox::send_message(const Message &msg) {
    std::lock_guard<std::mutex> lck (outbox_mutex);
    std::vector<uint8_t> raw_message = serialize(msg);
    auto promise = std::make_shared<std::promise<void>>();
    pending_receipts[packet_seq_id] = promise;
    socket.send(raw_message);
    // TODO: Insert timeout here
    return promise->get_future();
}

void MessageBox::process_events() {
    while(!inbox.empty()) {
        std::lock_guard<std::mutex> lck (inbox_mutex);
        auto msg = inbox.front();
        inbox.pop_front();

        auto callback = callback_map.find(msg.cmd_name);
        if(callback == callback_map.end()) {
            //std::cout << "Received unhandled \"" << msg.cmd_name << "\" packet\n";
        } else {
            //std::cout << "Received handled \"" << msg.cmd_name << "\" packet\n";
            callback->second(msg);
        }
    }
}

void MessageBox::event_loop() {
    while(handler_thread_running) {
        auto incoming = socket.recv();
        //hexdump(incoming);
        auto msg = Message(incoming);
        //std::cout << "msg has uid " << msg.uid << " ackid " << msg.ackid \
        // << " type " << msg.type << " seq_num " << msg.sequence_num << "\n";
        // Apparently this is the only way to tell if the initial state dump is over.
        // After this, the switcher will begin expecting acknowledgements of every
        // packet.
        if(!(this->is_initialized | msg.payload.size())) {
            std::cout << "Now we are initialized!\n";
            is_initialized = true;
            connection_promise.set_value();
        }

        if(msg.type & Message::Types::AckReq) {
            if(is_initialized) {
                *this << Message::ACKFrom(msg);
            }
            if(msg.payload.size()) {
                inbox_mutex.lock();
                inbox.push_back(msg);
                inbox_mutex.unlock();
            }
        }

        if(msg.type & Message::Types::Hello) {
            std::cout << "Received new hello packet\n";
            is_initialized = false;
            *this << Message(Message::Types::ACK);
        }

        if(msg.type & Message::Types::ACK) {
            auto pending_promise = pending_receipts.find(msg.ackid);
            if(pending_promise != pending_receipts.end()) {
                pending_receipts.erase(msg.ackid);
                pending_promise->second->set_value();
            }
        }
        // At this stage, after the relevant parsing callback has been invoked
        // to update the internal state of the class, we need to see if any get
        // or set functions have requested the results, and if so, trigger the
        // relevant callback or whatever
        session_id = msg.uid;
    }
    std::cout << "Leaving handler thread\n";
}

MessageBox::~MessageBox() {
    handler_thread_running = false;
    handler_thread.join();
}

MessageBox::MessageBox() {
}

void foo(const Message & msg) {
    std::cout << "lol wtf is going on  \n";
}

void MessageBox::registerCallback(const std::string & command_name, const callback_t & callback_function) {
    if(callback_map.find(command_name) != callback_map.end()) {
        std::cout << "Warning: Re-registering callback \"" << command_name << "\"!\n";
    } else {
        std::cout << "Registering callback \"" << command_name << "\"!\n";
    }
    callback_map[command_name] = callback_function;
}

void MessageBox::connect(std::string hostname) {
    socket.open(hostname);
    handler_thread = std::thread{&MessageBox::event_loop, this};
    *this << MakeHelloMessage();
    connection_promise.get_future().wait();
};

void MessageBox::operator<< (const Message & msg) {
    send_message(msg);
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
    uint16_t this_packet_seq_id = 0;

    // Hello and ACK packets do not increment the packet counter nor do they include it
    if(!(msg.type & (Message::Types::Hello | Message::Types::ACK))) this_packet_seq_id = ++packet_seq_id;

    // Message type and n_bytes are combined in the first word of the message
    append_word(raw_message, (msg.type << 11) | (msg.payload.size() + SIZE_OF_HEADER));
    append_word(raw_message, session_id);
    append_word(raw_message, msg.ackid); // ACK_ID ; dunno what this is yet
    append_word(raw_message, msg.sequence_num); // PackageID
    append_word(raw_message, 0); // Padding word
    append_word(raw_message, this_packet_seq_id); // PackageID
    raw_message.insert(raw_message.end(), msg.payload.begin(), msg.payload.end());
    //hexdump(raw_message);
    return raw_message;
};
