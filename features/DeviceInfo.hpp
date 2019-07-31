#include "network/MessageBox.hpp"

class ATEMDeviceInfo {
    private:
        MessageBox * mbox;
    public:
        ATEMDeviceInfo(MessageBox * mbox);
        void dummy_callback(const Message &message);
};
