#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Chip8.h"

using namespace std;

int getKeyCode(SDL_Keycode key) {
    switch (key) {
        case SDLK_1: {
            return 1;
        }
        break;
        case SDLK_2: {
            return 2;
        }
        break;
        case SDLK_3: {
            return 3;
        }
        break;
        case SDLK_4: {
            return 12;
        }
        break;
        case SDLK_Q: {
            return 4;
        }
        break;
        case SDLK_W: {
            return 5;
        }
        break;
        case SDLK_R: {
            return 13;
        }
        break;
        case SDLK_A: {
            return 7;
        }
        break;
        case SDLK_S: {
            return 8;
        }
        break;
        case SDLK_D: {
            return 9;
        }
        break;
        case SDLK_F: {
            return 14;
        }
        break;
        case SDLK_Z: {
            return 10;
        }
        break;
        case SDLK_X: {
            return 0;
        }
        break;
        case SDLK_C: {
            return 11;
        }
        break;
        case SDLK_V: {
            return 15;
        }
        break;
        case SDLK_E: {
            return 6;
        }
        break;
        default: {
            return -1;
        }
        break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr<<"Не все аргументы были получены"<<endl;
        return -1;
    }
    Chip8 chip8;
    int result = chip8.loadFromFile(argv[1]);
    if (result != 0) {
        cerr<<"Ошибка чтения rom"<<strerror(errno)<<endl;
        return 1;
    }
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

    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderScale(renderer, 10.0f, 10.0f);

    bool running = true;
    uint64_t last = SDL_GetTicks();
    double acc = 0.0;
    const double tickMs = 1000.0 / 60.0;
    while (running) {
        uint64_t now = SDL_GetTicks();
        acc += now - last;
        last = now;
        while (acc >= tickMs) {
            chip8.tickTimers();
            acc -= tickMs;
        }
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                int k = getKeyCode(event.key.key);
                if (k != -1) {
                    chip8.setKey(k, event.type == SDL_EVENT_KEY_DOWN);
                }
            }
        }
        for (int i = 0; i < 10; i++) {
            chip8.step();
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 50, 220, 120, 255);
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 32; y++) {
                if (chip8.screen[x][y] == 1) {
                    SDL_FRect pix {static_cast<float>(x), static_cast<float>(y), 1, 1};
                    SDL_RenderFillRect(renderer, &pix);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
