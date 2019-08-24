#pragma once
#include <memory>
#include <mutex>
#include <deque>
#include <future>
#include <map>
#define SIZE_OF_HEADER 0xc
#include <string>
#include <vector>
#include <functional>
#include "network/UDPSocket.hpp"
#include "network/Message.hpp"

#include <json.hpp>
using json = nlohmann::json;

class MessageBox {
    public:
        std::deque<Message> inbox;
        std::mutex inbox_mutex;
        std::mutex outbox_mutex;
        std::mutex pending_mutex;

        bool handler_thread_running = true;
        std::thread handler_thread;

        std::promise<void> connection_promise;

        typedef std::function<json(const Message & message)> callback_t;
        void registerCallback(const std::string & command_name, const callback_t & callback_function);

        void connect(std::string hostname);
        void operator<<(const Message &msg);
        std::string process_events();
        std::deque<std::string> event_messages;
        MessageBox();
        ~MessageBox();
        std::future<void> send_message(const Message &msg);
        std::map<uint16_t, std::shared_ptr<std::promise<void>>> pending_receipts;
    private:
        UDPSocket socket;
        bool is_initialized = false;
        uint16_t session_id = 0x1337; // The initial session id will be
                                      // changed by the mixer
        uint16_t packet_seq_id = 0;
        void sendHello();
        std::vector<uint8_t> serialize(const Message & msg);

        std::map<std::string, callback_t> callback_map;

        void event_loop();
};

void append_word(std::vector<uint8_t> & vector, uint16_t value);
