#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <vector>

Chip8::Chip8() {
    initialize();
}

void Chip8::initialize() {
    /**
     * programs start at address 0x200. 
     * 0x000 - 0x1FF is reserved for system level code (interpreter, fonts, etc.)
     */
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    gfx.fill(0);
    key.fill(0);

    delay_timer = 0;
    sound_timer = 0;

    const uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // fonts are loaded into memory starting at 0x000
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}

void Chip8::loadROM(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open ROM\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        for (size_t i = 0; i < buffer.size(); ++i) {
            memory[0x200 + i] = buffer[i];
        }
    }

    file.close();
}

void Chip8::emulateCycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000) {
        // ---- System instructions ----
        case 0x0000:
            // new mask for instructions
            switch (opcode && 0x00FF) {
                case 0x00E0: {
                    // clear the screen
                    gfx.fill(0);
                    drawFlag = true;
                    pc += 2;
                    break;
                }
            
                case 0x00EE: {
                    // return to a subroutine
                    --sp;
                    pc = stack[sp];
                    pc += 2;
                    break;
                }
                
                default: {
                    std::cerr << "Unkown 0x0000 opcode: " << std::hex << opcode << "\n";
                    pc += 2;
                    break;
                }
            }
            // break from outer switch case
            break;
        // ---- Default ----
        case 0x1000: {
            // call a subroutine
            pc = opcode & 0x0FFF;
            break;
        }
        case 0x2000: {
            // change program counter to to new position
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        }
        case 0x3000: {
            // conditional statement to skip current instruction
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x4000: {
            // conditional statement - check for NOT equals here
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x5000: {
            // conditional check
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        case 0x9000: {
            // conditional check - NOT
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        }
        // ---- Arithmetic & Logic ----
        case 0x6000: {
            // set registry value (to last 2 digits of opcode)
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        }
        case 0x7000: {
            // add value to registry
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
        }
        case 0x8000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                case 0x0: { V[x] = V[y]; break; } // assign
                case 0x1: { V[x] |= V[y]; break; } // OR
                case 0x2: { V[x] &= V[y]; break; } // AND
                case 0x3: { V[x] ^= V[y]; break; } // XOR
                case 0x4: {
                    uint16_t sum = V[x] + V[y];
                    V[0xF] = (sum > 255) ? 1 : 0;
                    V[x] = sum & 0xFF; // add remainder of 255, if any
                    break;
                }
                case 0x5: {
                    V[0xF] = (V[x] > V[y]) ? 1 : 0;
                    V[x] -= V[y];
                    break;
                }
                case 0x6: {
                    V[0xF] = V[x] & 0x1; // store LSB
                    V[x] >>= 1;
                    break;
                }
                case 0x7: {
                    V[0xF] = (V[y] > V[x]) ? 1 : 0;
                    V[x] = V[y] - V[x];
                    break;
                }
                case 0xE: {
                    V[0xF] = (V[x] & 0x80) >> 7; // isolate MSB and shift right, setting carry flag
                    V[x] >>= 1; // right bit shift, which divides value by 2
                    break;
                }
                default: {
                    std::cerr << "Unkown 0x8000 opcode: " << std::hex << opcode << "\n";
                    break;
                }
            }
            pc += 2;
            break;
        }
        // ---- Memory, Random, & Display ----
        case 0xA000: {
            // set index register to last 3 hex values
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        }
        case 0xB000: {
            // assign program counter value
            pc = (opcode & 0x0FFF) + V[0];
            break;
        }
        case 0xC000: {
            // create random number and store in V[x]
            V[(opcode & 0x0F00) >> 8] = (rand() % 255) & (opcode & 0x00FF);
            pc += 2;
        }
        case 0xD000: {
            uint8_t x = V[(opcode & 0x0F00) >> 8];
            uint8_t y = V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;
            V[0xF] = 0;

            for (int row = 0; row < height; ++row) {
                uint8_t sprite = memory[I + row];
                for (int col = 0; col < 8; ++col) {
                    if ((sprite & (0x80 >> col)) != 0) {
                        int index = (x + col + ((y + row) * 64)) % (64 * 32);
                        if (gfx[index] == 1) V[0xF] = 1;
                        gfx[index] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            break;
        }
        // ---- Keys & Timers ----
        case 0xE000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                // if key Vx is pressed, skip
                case 0x9E: {
                    key[V[x]] ? pc += 4 : pc += 2;
                    break;
                }
                case 0xA1: {
                    !key[V[x]] ? pc += 4 : pc += 2;
                    break;
                }
                default: {
                    std::cerr << "Unkown 0xE000 opcode: " << std::hex << opcode << "\n";
                    pc += 2;
                    break;
                }
            }
            break;
        }
        case 0xF000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07: { V[x] = delay_timer; break; }
                case 0x0A: {
                    bool key_pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (key[i]) {
                            V[x] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed) return; // wait for key press
                    break;
                }
                case 0x15: { delay_timer = V[x]; break; }
                case 0x18: { sound_timer = V[x]; break; }
                case 0x1E: { I += V[x]; break; }
                case 0x29: { I = V[x] * 5; break; }
                case 0x33: {
                    memory[I] = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = V[x] % 10;
                    break;
                }
                case 0x55: {
                    for (int i = 0; i <= x; ++i) {
                        memory[I + i] = V[i];
                    }
                    break;
                }
                case 0x65: {
                    for (int i = 0; i <= x; ++i) {
                        V[i] = memory[I + i];
                    }
                    break;
                }
                default: {
                    std::cerr << "Unkown 0xF000 key opcode: " << std::hex << opcode << "\n";
                    break;
                }
            }
            pc += 2;
            break;
        }

        default: {
            std::cerr << "Unknown opcode: " << std::hex << opcode << "\n";
            pc += 2;
            break;
        }
            
        // timer countdown and defaults
        if (delay_timer > 0) --delay_timer;
        if (sound_timer > 0) {
            --sound_timer;
            if (sound_timer == 0) {
                // TODO: implement audio cue play
                std::cout << "BEEP!\n";
            }
        }
    }
}