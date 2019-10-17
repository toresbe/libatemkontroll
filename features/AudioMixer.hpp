#include "network/MessageBox.hpp" 

class ATEMAudioMixerInput { 
    public:
        enum status_t : uint8_t {
            Off,
            On,
            AFV
        };

        enum type_t : uint8_t {
            ExternalVideo,
            MediaPlayer,
            ExternalAudio,
        };

        enum plugtype_t : uint8_t {
            Internal  = 0,   
            SDI       = 1,   
            HDMI      = 2,   
            Component = 3,   
            Composite = 4,   
            SVideo    = 5,   
            XLR       = 32,  
            AES_EBU   = 64,  
            RCA       = 128, 
        };
    private:
        uint16_t input_index;
        MessageBox *mbox;
        type_t type;
        plugtype_t plugtype;
        status_t status;
        uint16_t gain;
        int16_t pan;
    public:

        ATEMAudioMixerInput(MessageBox * mbox, std::vector<uint8_t> AMIP_payload);
        //set_gain(uint16_t gain);
        status_t get_status();
        type_t get_type();
        plugtype_t get_plugtype();
        uint16_t get_gain();
        int16_t get_pan();

        void set_status(status_t new_status);
};

class ATEMAudioMixer {
    private:
        MessageBox * mbox;
        unsigned int num_inputs = 0;
        bool has_monitor_out = false;
    public:
        std::map<uint16_t, std::shared_ptr<ATEMAudioMixerInput>> inputs;
        const std::map<uint16_t, std::string> input_names = {
            {1, "Input 1"},
            {2, "Input 2"},
            {3, "Input 3"},
            {4, "Input 4"},
            {5, "Input 5"},
            {6, "Input 6"},
            {7, "Input 7"},
            {8, "Input 8"},
            {9, "Input 9"},
            {10, "Input 10"},
            {11, "Input 11"},
            {12, "Input 12"},
            {13, "Input 13"},
            {14, "Input 14"},
            {15, "Input 15"},
            {16, "Input 16"},
            {17, "Input 17"},
            {18, "Input 18"},
            {19, "Input 19"},
            {20, "Input 20"},
            {1001, "XLR"},
            {1101, "AES/EBU"},
            {1201, "RCA"},
            {2001, "MP1"},
            {2002, "MP2"}
        };
        ATEMAudioMixer(MessageBox * mbox);
        json handle__AMC(const Message &message); // Audio Mixer Configuration
        json handle_AMIP(const Message &message); // Audio Mixer Input
};


