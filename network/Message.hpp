#pragma once
#include <vector>
#include <cstdint>
#include <string>

class Message {
    private:

    public:
        enum Types {
            NoCmd = 0x0,
            AckReq = 0x1,
            Hello = 0x2,
            Resend = 0x4,
            Undef = 0x8,
            ACK = 0x10
        };

        std::vector<uint8_t> payload;
        int type = 0;
        int uid = 0;
        int ackid = 0;
        int sequence_num = 0;
        std::string cmd_name;
        Message(std::vector<uint8_t> raw_message);
        Message(Types type);
        static Message ACKFrom(const Message & msg);
        static Message Command(std::string cmd_name, std::vector<uint8_t> payload);
        Message();
        void build_cmd_payload(const std::vector<uint8_t> &payload);
};
Message MakeHelloMessage();
