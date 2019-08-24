#include "network/MessageBox.hpp"

class ATEMDeviceInfo {
    private:
        MessageBox * mbox;
    public:
        unsigned int num_MEs, num_stingers, num_srcs, num_colorgens;
        unsigned int num_DVEs, num_auxes, num_DSKs, num_supersrcs;
        bool has_SD_output;
        ATEMDeviceInfo(MessageBox * mbox);
        void handle_state_change(const Message &message);
        void dummy_callback(const Message &message);
        void parse_topology(const Message &message);
};
