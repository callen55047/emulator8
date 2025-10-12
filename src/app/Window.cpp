//
// Created by Callen Egan on 2025-10-10.
//

#include "app/Window.h"

Window::Window(int width, int height, int scale)
    : width(width), height(height), scale(scale) {}

Window::~Window() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::init(const char* title) {
    // for running on MacOS
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    window = SDL_CreateWindow(title, width * scale, height * scale, 0);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        return false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
        return false;
    }

    initialized = true;
    return true;
}

void Window::drawFrame(const uint8_t* gfx) {
    if (!initialized) return;

    uint32_t pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT];
    for (int i = 0; i < width * height; ++i) {
        uint8_t pixel = gfx[i];
        pixels[i] = pixel ? 0xFFFFFFFF : 0xFF000000;
    }

    SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}