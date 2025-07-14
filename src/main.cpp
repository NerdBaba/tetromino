#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <algorithm> // For std::min
#include "Game.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) { // Initialize audio subsystem
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Default window dimensions
    int windowWidth = Board::WIDTH * 20 + 150; // Board width * default cell size + space for score
    int windowHeight = Board::HEIGHT * 20; // Board height * default cell size

    // Parse command-line arguments for resolution
    if (argc >= 3) {
        try {
            windowWidth = std::stoi(argv[1]);
            windowHeight = std::stoi(argv[2]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid resolution arguments. Using default. Error: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Resolution arguments out of range. Using default. Error: " << e.what() << std::endl;
        }
    }

    SDL_Window* window = SDL_CreateWindow("Tetris Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN); 
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Game game;
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            game.handleInput(e);
        }

        game.update();

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Calculate cellSize based on window dimensions
        int currentWindowWidth, currentWindowHeight;
        SDL_GetWindowSize(window, &currentWindowWidth, &currentWindowHeight);
        int cellSize = std::min(currentWindowWidth / Board::WIDTH, currentWindowHeight / Board::HEIGHT);

        game.render(renderer, cellSize);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio(); // Close SDL_mixer
    TTF_Quit();
    SDL_Quit();

    return 0;
}