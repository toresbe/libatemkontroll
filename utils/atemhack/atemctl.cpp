#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "atem.hpp"
#include <cstring>
#include <string>
#include "loguru.hpp"
#include <sys/select.h>

#include <json.hpp>
using json = nlohmann::json;

uint16_t read_input_index(char *str) {
    int input_index;
    try {
        input_index = std::stoi(str);
    } catch (std::exception &e) {
        exit(1);
    }
    return (uint16_t) input_index;
}

bool input_available() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

int command(ATEM &atem, int bus, int input) {
    //#uint16_t input_index = read_input_index("2");
    uint16_t input_index = input;
    if (input_index == 23) input_index = 1000;
    if(bus == 0) {
        atem.MixEffects.set_preview(input_index);
    }
    else if(bus == 1) {
        atem.MixEffects.set_program(input_index);
    }
    else if(bus == 2) {
        atem.MixEffects.set_preview(input_index);
    }
}

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Starting up");
    ATEM atem;
    atem.connect("10.3.2.1");

    bool running = true;
    while(running) 
        if(input_available()) {
            int output, input;
            std::string line; 
            std::getline(std::cin, line); 
            if(sscanf(line.c_str(), "%d, %d", &input, &output) == 2) {
                LOG_F(1, "Got command: output bus %d -> input %d", output, input);
                command(atem, output, input);
            } else {
                LOG_F(WARNING, "SYNTAX ERROR.\n");
            }
        }
    auto events_string = atem.process_events();
    json events;
    try {
        events = events_string;
    } catch (std::exception &e) {
                LOG_F(WARNING, "Invalid JSON returned.");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
