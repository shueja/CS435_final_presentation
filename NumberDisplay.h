#include <mbed.h>
#include <ShiftReg.h>

// main() runs in its own thread in the OS
#define SH D10
#define ST D9
#define DS D8


uint8_t Number(uint8_t floor) {
    switch (floor) {
        case 0: return 0b11111100;
        case 1: return 0b01100000;
        case 2: return 0b11011010;
        case 3: return 0b11110010;
        case 4: return 0b01100110;
        default: return 0b00000000;
    }
}
class NumberDisplay {

    
    public:
        NumberDisplay(PinName data, PinName store, PinName shift) : 
            reg(data, store, shift) {

            }
        

        void Show(uint8_t num) {
            number = num;
            Update();
        }
        void Point(bool show) {
            point = show;
            Update();
        }
    private:
        ShiftReg reg;
        uint8_t number = 0b00000000;
        bool point;
        void Update() {
            uint8_t byt = number + (point ? 1 : 0);
            reg.ShiftByte(~byt);
            reg.Latch();
        }
};