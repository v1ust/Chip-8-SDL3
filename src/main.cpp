#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Тестовая программа: проверяем, что SDL3 подключён и окно открывается.
// Закрыть: крестик окна или Escape.
int main(int, char*[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Не удалось инициализировать SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("CHIP-8", 640, 320, 0, &window, &renderer)) {
        SDL_Log("Не удалось создать окно: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderVSync(renderer, 1);  // не грузить процессор на 100%

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 50, 220, 120, 255);
        SDL_FRect square{270.0f, 110.0f, 100.0f, 100.0f};
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
