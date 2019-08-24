#include "network/MessageBox.hpp" 
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ATEMMixEffects {
    private:
        MessageBox * mbox;
        uint8_t me_index = 0;
        typedef struct me_state {
            uint16_t program_input;
        } me_state;
        me_state state[8];
    public:
        ATEMMixEffects(MessageBox * mbox);
        void set_program(uint16_t input_index);
        void set_preview(uint16_t input_index);
        void take_auto();
        json handle_PrgI(const Message &message);
        json handle_PrvI(const Message &message);
        void take_cut();
};
