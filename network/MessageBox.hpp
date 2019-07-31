#pragma once
#include <map>
#define SIZE_OF_HEADER 0xc
#include <string>
#include <vector>
#include <functional>
#include "network/UDPSocket.hpp"
#include "network/Message.hpp"

class MessageBox {
    public:
        typedef std::function<void(const Message & message)> callback_t;
        void registerCallback(const std::string & command_name, const callback_t & callback_function);
        void connect(std::string hostname);
        void operator<<(const Message &msg);
        void parse_datagram(const std::vector<uint8_t> &dgram);
        void process_events();
        MessageBox();
    private:
        bool is_initialized = false;
        uint16_t current_UID = 0x1337;
        uint16_t packet_seq_id = 0;
        UDPSocket socket;
        void sendHello();
        std::vector<uint8_t> serialize(const Message & msg);
        typedef std::map<std::string, callback_t> callback_map_t;
        callback_map_t callback_map;
};
