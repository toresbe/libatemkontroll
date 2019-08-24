#include <string>
#include "network/MessageBox.hpp"
#include "features/MixEffects.hpp"
#include "features/DeviceInfo.hpp"

class ATEM {
    private:
        MessageBox mbox;
    public:
        ATEMDeviceInfo DeviceInfo;
        ATEMMixEffects MixEffects;
        void connect(std::string hostname);
        std::string process_events();
        ATEM();
};
