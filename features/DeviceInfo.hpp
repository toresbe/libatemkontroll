#include "network/MessageBox.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <json.hpp>
using json = nlohmann::json;

class ATEMDeviceInfo {
    private:
        MessageBox * mbox;
    public:
        unsigned int num_MEs, num_stingers, num_srcs, num_colorgens;
        unsigned int num_DVEs, num_auxes, num_DSKs, num_supersrcs;
        bool has_SD_output;
        ATEMDeviceInfo(MessageBox * mbox);
        json handle_state_timecode(const Message &message);
        json dummy_callback(const Message &message);
        json parse_topology(const Message &message);
};
