#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tetromino.h"
#include <random>
#include <SDL_ttf.h>
#include <SDL_mixer.h> // Include SDL_mixer

class Game {
public:
    Game();
    ~Game(); // Destructor to clean up font and mixer

    void handleInput(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer, int cellSize); // Added cellSize parameter

    bool isGameOver() const { return gameOver; }
    int getScore() const { return score; }
    int getLevel() const { return level; }

private:
    Board board;
    Tetromino currentTetromino;
    Tetromino nextTetromino;
    Tetromino heldTetromino; // New: for holding pieces

    int score;
    int level;
    bool gameOver;
    bool paused; // New: for pause menu
    bool canSwap; // New: to limit swapping to once per piece

    Uint32 lastFallTime;
    Uint32 fallDelay;

    std::mt19937 rng; // Random number generator
    TTF_Font* font; // Font for rendering text
    Mix_Music* backgroundMusic; // Background music
    Mix_Chunk* moveSound;       // Sound for movement
    Mix_Chunk* scoreSound;      // Sound for scoring points
    Mix_Chunk* gameOverSound;   // Sound for game over

    void spawnTetromino();
    bool moveTetromino(int dx, int dy);
    void rotateTetromino();
    void lockTetromino();
    void updateScore(int linesCleared);
    void checkGameOver();
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color); // Helper for rendering text
    void renderNextTetromino(SDL_Renderer* renderer, int cellSize, int xOffset, int yOffset); // Helper for next tetromino
    void renderHeldTetromino(SDL_Renderer* renderer, int cellSize, int xOffset, int yOffset); // New: Helper for held tetromino
    void swapTetromino(); // New: for swapping pieces
};

#endif // GAME_H