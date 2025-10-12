//
// Created by Callen Egan on 2025-10-12.
//

#include "app/Menu.h"
#include "App.h"
#include <SDL3/SDL.h>
#include <iostream>

// Basic white text on black background placeholder menu
Menu::Menu() {}

void Menu::handleEvent(const SDL_Event& event, AppState& state) {

    if (event.type == SDL_EVENT_KEY_DOWN) {
        // TODO: save key maps in constants class?
        switch (event.key.key) {
            case SDLK_UP:
                selectedIndex = (selectedIndex + 1) % 2;
                break;
            case SDLK_DOWN:
                selectedIndex = (selectedIndex + 1) % 2;
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (selectedIndex == 0) {
                    std::cout << "Loading ROM (placeholder)...\n";
                    state = AppState::Running;
                } else if (selectedIndex == 1) {
                    state = AppState::Quit;
                }
                break;
            default:
                break;
        }
    }
}

void Menu::render(Window& window) {
    SDL_Renderer* renderer = window.getRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a simple title
    drawCenteredText(window, "CHIP-8 Emulator", 50);
    for (int i = 0; i < 2; ++i) {
        drawCenteredText(window, options[i], 120 + i * 40, i == selectedIndex);
    }
}

void Menu::renderPauseOverlay(Window& window) {
    SDL_Renderer* renderer = window.getRenderer();

    // Dim the screen with a translucent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, nullptr);

    drawCenteredText(window, "Paused", 80);
    drawCenteredText(window, "Press ESC to resume", 130);
    drawCenteredText(window, "Press Q to quit", 170);
}

void Menu::drawCenteredText(Window& window, const std::string& text, int y, bool selected) {
    // NOTE: This is a placeholder — we'll replace it with SDL_ttf text rendering soon.
    SDL_Renderer* renderer = window.getRenderer();

    SDL_Rect rect;
    rect.w = 8 * static_cast<int>(text.size());
    rect.h = 20;
    rect.x = (window.getWidth() - rect.w) / 2;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, selected ? 255 : 200, selected ? 255 : 200, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}