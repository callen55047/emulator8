#include "App.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

App::App(int width, int height, int scale) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    window = std::make_unique<Window>(width, height, scale);
    emulator = std::make_unique<Emulator>();
    menu = std::make_unique<Menu>();
}

App::~App() {
    SDL_Quit();
}

void App::run() {
    while (running && state != AppState::Quit) {
        processEvents();
        update();
        render();
    }
}

void App::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
            state = AppState::Quit;
        }

        switch (state) {
            case AppState::MainMenu:
                menu->handleEvent(event, state);
                break;
            case AppState::Running:
            case AppState::Paused:
                emulator->handleEvent(event, state);
                break;
            default:
                break;
        }
    }
}

void App::update() {
    switch (state) {
        case AppState::Running:
            emulator->update();
            break;
        default:
            break;
    }
}

void App::render() {
    window->clear();

    switch (state) {
        case AppState::MainMenu:
            menu->render(*window);
            break;
        case AppState::Running:
        case AppState::Paused:
            emulator->render(*window);
            if (state == AppState::Paused)
                menu->renderPauseOverlay(*window);
            break;
        default:
            break;
    }

    window->present();
}