//
// Created by Callen Egan on 2025-10-08.
//
#pragma once

#include "app/Window.h"
#include <memory>
#include "app/Emulator.h"
#include "app/Menu.h"

enum class AppState {
    MainMenu,
    Running,
    Paused,
    Quit
};

class App {
public:
    App(int width = 64, int height = 32, int scale = 10);
    ~App();

    void run();

private:
    void processEvents();
    void update();
    void render();

    std::unique_ptr<Window> window;
    std::unique_ptr<Emulator> emulator;
    std::unique_ptr<Menu> menu;

    AppState state = AppState::MainMenu;
    bool running = true;
};