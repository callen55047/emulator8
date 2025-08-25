#include <array>
#include <cstdint>

class Chip8 {
public:
    Chip8();

    void loadROM(const char* filename);
    void debugMemory();
    void emulateCycle(); // do one instruction cycle
    
    bool drawFlag;
    std::array<uint8_t, 2048> gfx; // 64x32 monochrome display
    std::array<uint8_t, 16> key; // HEX keypad

private:
    std::array<uint8_t, 4096> memory; // 4K memory
    std::array<uint8_t, 16> V; // 16 general purpose registers
    std::array<uint16_t, 16> stack; // call stack

    uint16_t opcode; // current instruction
    uint16_t sp; // stack pointer
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t I; // index register
    uint16_t pc; // program counter

    void initialize(); // reset emulator state
};
