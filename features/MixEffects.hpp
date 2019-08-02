#include "network/MessageBox.hpp" 

class ATEMMixEffects {
    private:
        MessageBox * mbox;
    public:
        ATEMMixEffects(MessageBox * mbox);
        void set_program(uint16_t input_index);
};
