#include <iostream>
#include "atem.hpp"
#include <cstring>
#include <string>

void usage() {
    std::cout << "Usage:\n";
    std::cout << "\tatemctl [program|preview] [input_index]\n";
    std::cout << "\tatemctl take [auto|cut]\n";
    std::cout << "\tatemctl nop\n";
}

uint16_t read_input_index(char *str) {
    int input_index;
    try {
        input_index = std::stoi(str);
    } catch (std::exception &e) {
        usage(); 
        exit(1);
    }
    return (uint16_t) input_index;
}

int main(int argc, char *argv[]) {
    ATEM atem;
    if(argc != 3) {
        if((argc == 2) && !strcmp(argv[1], "nop")) {
            atem.connect("10.3.2.1");
        } else {
            usage();
            exit(1);
        }
    } else {
        if(!strcmp(argv[1], "program")) {
            uint16_t input_index = read_input_index(argv[2]);
            atem.connect("10.3.2.1");
            atem.MixEffects.set_program((uint16_t) input_index);
        } else if(!strcmp(argv[1], "preview")) {
            uint16_t input_index = read_input_index(argv[2]);
            atem.connect("10.3.2.1");
            atem.MixEffects.set_preview((uint16_t) input_index);
        } else if(!strcmp(argv[1], "take")) {
            if(!strcmp(argv[2], "auto")) {
                atem.connect("10.3.2.1");
                atem.MixEffects.take_auto();
            } else if(!strcmp(argv[2], "cut")) {
                atem.connect("10.3.2.1");
                atem.MixEffects.take_cut();
            }
        }
    }
    while(1) {
        atem.process_events();
    };
    return(0);
}
