#include <iostream>
#include "atem.hpp"
#include <cstring>
#include <string>


uint16_t read_input_index(char *str) {
    int input_index;
    try {
        input_index = std::stoi(str);
    } catch (std::exception &e) {
        exit(1);
    }
    return (uint16_t) input_index;
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
    ATEM atem;
    atem.connect("10.3.2.1");
    int output, input;
    for (std::string line; std::getline(std::cin, line);) {
        atem.process_events();
        if(sscanf(line.c_str(), "%d, %d", &input, &output) == 2) {
            printf("Got command: output bus %d -> input %d\n", output, input);
            command(atem, output, input);
        }
    }
}
