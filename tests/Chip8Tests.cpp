#include "../include/app/emulator/Chip8.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace {

bool expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
    }
    return condition;
}

bool testLoadRegisterImmediate() {
    Chip8 chip;
    chip.loadProgram({0x60, 0x0A}); // LD V0, 0x0A
    chip.emulateCycle();

    bool ok = true;
    ok &= expect(chip.getRegister(0) == 0x0A, "LD Vx, byte should set register");
    ok &= expect(chip.getPC() == 0x202, "Program counter should advance by 2");
    return ok;
}

bool testAddToRegister() {
    Chip8 chip;
    chip.loadProgram({0x60, 0x05, 0x70, 0x03}); // LD V0, 0x05; ADD V0, 0x03
    chip.emulateCycle();
    chip.emulateCycle();

    bool ok = true;
    ok &= expect(chip.getRegister(0) == 0x08, "ADD Vx, byte should increment register");
    ok &= expect(chip.getPC() == 0x204, "Program counter should advance across two instructions");
    return ok;
}

bool testCallAndReturn() {
    Chip8 chip;
    chip.loadProgram({0x22, 0x04, 0x60, 0x01, 0x00, 0xEE});
    chip.emulateCycle();

    bool ok = true;
    ok &= expect(chip.getPC() == 0x204, "CALL should jump to subroutine address");

    chip.emulateCycle();
    ok &= expect(chip.getPC() == 0x202, "RET should jump back to caller address");
    return ok;
}

bool testClearScreen() {
    Chip8 chip;
    chip.loadProgram({0x00, 0xE0});
    chip.gfx.fill(0xFF);

    chip.emulateCycle();

    bool ok = true;
    ok &= expect(chip.drawFlag, "CLS should set draw flag");
    ok &= expect(std::all_of(chip.gfx.begin(), chip.gfx.end(), [](uint8_t pixel) { return pixel == 0; }),
                 "CLS should zero the framebuffer");
    return ok;
}

} // namespace

int main() {
    bool ok = true;
    ok &= testLoadRegisterImmediate();
    ok &= testAddToRegister();
    ok &= testCallAndReturn();
    ok &= testClearScreen();

    if (!ok) {
        std::cerr << "\nChip8 core tests failed." << std::endl;
        return 1;
    }

    std::cout << "Chip8 core tests passed." << std::endl;
    return 0;
}
