#include "App.h"
#include "../include/app/emulator/Chip8.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <vector>

#include "app/Window.h"

namespace fs = std::filesystem;

void App::init() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        currentState = AppState::EXIT;
        return;
    }

    if (TTF_Init()) {
        std::cerr << "TTF_Init: " << SDL_GetError() << std::endl;
        SDL_Quit();
        currentState = AppState::EXIT;
        return;
    }

    window = Window();

    // Load available ROM filenames
    std::string roms_path = "roms";
    try {
        for (const auto& entry : fs::directory_iterator(roms_path)) {
            if (entry.is_regular_file()) {
                roms.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to read roms directory: " << e.what() << std::endl;
    }
}

void App::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            currentState = AppState::EXIT;
        } else if (e.type == SDL_EVENT_KEY_DOWN) {
            switch (e.key.key) {
                case SDLK_UP:
                    selectedRomIndex = (selectedRomIndex - 1 + roms.size()) % roms.size();
                    break;
                case SDLK_DOWN:
                    selectedRomIndex = (selectedRomIndex + 1) % roms.size();
                    break;
                case SDLK_RETURN:
                    currentState = AppState::RUNNING;
                    break;
                case SDLK_ESCAPE:
                    // TODO: add overlay for menu to exit
                    currentState = AppState::EXIT;
                    break;
            }
        }
    }
}

void App::update() {
    // Nothing dynamic to update in menu yet
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load font once per render — ideally move this to init() for performance
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};

    int y = 100;
    for (int i = 0; i < (int)roms.size(); ++i) {
        const char* text = roms[i].c_str();
        SDL_Surface* surface = TTF_RenderText_Solid(font, text, strlen(text), white);
        if (!surface) continue;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_DestroySurface(surface);
            continue;
        }

        int texW = surface->w;
        int texH = surface->h;
        SDL_FRect dstrect = {100.0f, (float)y, (float)texW, (float)texH};

        // Highlight the selected ROM
        if (i == selectedRomIndex) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            SDL_RenderFillRect(renderer, &dstrect);
        }

        SDL_RenderTexture(renderer, texture, nullptr, &dstrect);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
        y += 50;
    }

    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
}

void App::cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void App::run() {
    init();

    while (currentState != AppState::EXIT) {
        handleEvents();
        if (currentState == AppState::RUNNING) {
            Chip8 chip8;
            // TODO: create global computed var for current ROM path
            chip8.loadROM(("roms/" + roms[selectedRomIndex]).c_str());

            // Game loop
            bool quit = false;
            while (!quit) {
                chip8.emulateCycle();

                if (chip8.drawFlag) {
                    chip8.emulateCycle();
                }

                // chip8.setKeys(); - pass in input for emulator?

                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_EVENT_QUIT) {
                        quit = true;
                        break;
                    }
                }
            }

            currentState = AppState::MENU;
        } else {
            update();
            render();
        }
    }

    cleanup();
}