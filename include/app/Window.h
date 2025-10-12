//
// Created by Callen Egan on 2025-10-10.
//

#ifndef CHIP8_EMULATOR_WINDOW_H
#define CHIP8_EMULATOR_WINDOW_H

#endif //CHIP8_EMULATOR_WINDOW_H

#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>

class Window {
public:
    static constexpr int DEFAULT_WIDTH = 64;
    static constexpr int DEFAULT_HEIGHT = 32;
    static constexpr int DEFAULT_SCALE = 10;

    Window(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT, int scale = DEFAULT_SCALE);
    ~Window();

    bool init(const char* title = "Chip-8 Emulator");
    void drawFrame(const uint8_t* gfx);
    bool isInitialized() const { return initialized; }

private:
    int width, height, scale;
    bool initialized = false;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
};