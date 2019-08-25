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

class ButtonInputMap {
    public:
        std::map<unsigned int, unsigned int> map = {
            {0, 0},
            {1, 1},
            {2, 2},
            {23, 1000},
        };

    unsigned int get_input(unsigned int button) {
        if(map.count(button)) {
            return map[button];
        }
        return 0;
    }

    unsigned int get_button(unsigned int input) {
        for (auto x: map) {
            if(x.second == input) {
                return x.first;
            }
        }
        return 0;
    }
};

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
    ButtonInputMap DVSMap;
    atem.connect("10.3.2.1");

    bool running = true;
    while(running) {
        if(input_available()) {
            int output, input;
            std::string line; 
            std::getline(std::cin, line); 
            if(sscanf(line.c_str(), "%d, %d", &input, &output) == 2) {
                LOG_F(1, "Got command: output bus %d -> input %d", output, input);
                auto input_index = DVSMap.get_input(input);
                command(atem, output, input_index);
            } else {
                LOG_F(WARNING, "SYNTAX ERROR");
            }
        }
        auto events_string = atem.process_events();
        json events;
        try {
            if(events_string != "") {
                events = json::parse(events_string);
                LOG_F(INFO, events_string.c_str());
            }
        } catch (std::exception &e) {
            LOG_F(WARNING, "Invalid JSON returned.");
        }
        if(events["module"] == "ME" && events["subject"] == "preview_input_changed") {
            auto button = DVSMap.get_button(events["input_index"]);
            std::cout << button << ", 0, 1" << "\n";
        } else if(events["module"] == "ME" && events["subject"] == "program_input_changed") {
            auto button = DVSMap.get_button(events["input_index"]);
            std::cout << button << ", 1, 1" << "\n";
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
