//
// Created by Callen Egan on 2025-10-12.
//

#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "app/Window.h"

enum class AppState;

class Menu {
public:
    Menu();
    ~Menu() = default;

    void handleEvent(const SDL_Event& event, AppState& state);
    void render(Window& window);
    void renderPauseOverlay(Window& window);

private:
    int selectedIndex = 0;
    const std::string options[2] = { "Load ROM", "Exit" };

    void drawCenteredText(Window& window, const std::string& text, int y, bool selected = false);
};