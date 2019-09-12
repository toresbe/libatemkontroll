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
            {3, 3},
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

class TransButtonInputMap {
    public:
        std::map<unsigned int, std::string> map = {
           {46, "auto"},
           {47, "cut"}
        };

    std::string get_input(unsigned int button) {
        if(map.count(button)) {
            return map[button];
        }
        return "";
    }

    unsigned int get_button(std::string input) {
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

void me_command(ATEM &atem, int bus, int input) {
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

void trans_command(ATEM &atem, std::string input) {
    if (input == "cut"){
        atem.MixEffects.take_cut();
    }
    if (input == "auto"){
        atem.MixEffects.take_auto();
    }
}

int main(int argc, char *argv[]) {
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Starting up");
    ATEM atem;
    ButtonInputMap DVSMap;
    TransButtonInputMap TransMap;
    atem.connect("10.73.1.100");

    bool running = true;
    while(running) {
        if(input_available()) {
            int output, input;
            char function[5];
            std::string line; 
            std::getline(std::cin, line); 
            if(sscanf(line.c_str(), "%s %d, %d", function, &input, &output) == 3) {
                if(strncmp(function, "me0:", 3)==0){
                    LOG_F(1, "Got command: output bus %d -> input %d", output, input);
                    auto input_index = DVSMap.get_input(input);
                    me_command(atem, output, input_index);
                }
                if(strncmp(function, "ta0:", 4)==0){
                    LOG_F(1, "Got trans key %d/%d", output, input);
                    // concat; ex: 4, 7 ==> 47
                    int index = (input * 10)+output;

                    auto cmd = TransMap.get_input(index);
                    trans_command(atem, cmd);
                }
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
        if(events["module"] == "ME"){
            if(events["subject"] == "preview_input_changed") {
                auto button = DVSMap.get_button(events["input_index"]);
                std::cout << "me0: " << button << ", 0, 1" << "\n";
            } else if(events["subject"] == "program_input_changed") {
                auto button = DVSMap.get_button(events["input_index"]);
                std::cout << "me0: " << button << ", 1, 1" << "\n";
            } else if(events["subject"] == "new_transition_position") {
                std::cout << "tb0: " << events["position"] << ", " << events["frames_remaining"] << ", " << events["active"] << "\n";
            }

        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
