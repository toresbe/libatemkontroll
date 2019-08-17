#include "network/MessageBox.hpp" 

class ATEMMixEffects {
    private:
        MessageBox * mbox;
        uint8_t me_index = 0;
    public:
        ATEMMixEffects(MessageBox * mbox);
        void set_program(uint16_t input_index);
        void set_preview(uint16_t input_index);
        void take_auto();
        void take_cut();
};
