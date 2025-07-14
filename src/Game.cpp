#include "Game.h"
#include <iostream>
#include <chrono>
#include <string>

Game::Game() : score(0), level(1), gameOver(false), paused(false), canSwap(true), lastFallTime(SDL_GetTicks()), fallDelay(1000) {
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    spawnTetromino();

    font = TTF_OpenFont("build/Array-Regular.otf", 24); // Load font
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}

Game::~Game() {
    if (font) {
        TTF_CloseFont(font);
    }
}

void Game::spawnTetromino() {
    currentTetromino = nextTetromino.getType() == TetrominoType::None ? Tetromino(static_cast<TetrominoType>(std::uniform_int_distribution<int>(0, static_cast<int>(TetrominoType::L))(rng))) : nextTetromino;
    nextTetromino = Tetromino(static_cast<TetrominoType>(std::uniform_int_distribution<int>(0, static_cast<int>(TetrominoType::L))(rng)));
    canSwap = true; // Reset swap ability for new piece

    if (board.isCollision(currentTetromino)) {
        gameOver = true;
    }
}

void Game::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            paused = !paused;
            return; // Handle pause key immediately
        }

        if (gameOver || paused) {
            return; // Ignore other inputs if game over or paused
        }

        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                moveTetromino(-1, 0);
                break;
            case SDLK_RIGHT:
                moveTetromino(1, 0);
                break;
            case SDLK_DOWN:
                if (!moveTetromino(0, 1)) {
                    lockTetromino();
                }
                break;
            case SDLK_UP:
                rotateTetromino();
                break;
            case SDLK_SPACE: // Hard drop
                while (moveTetromino(0, 1)) {
                    // Keep moving down until collision
                }
                lockTetromino();
                break;
            case SDLK_RETURN:
                swapTetromino();
                break;
        }
    }
}

void Game::update() {
    if (gameOver || paused) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFallTime > fallDelay) {
        if (!moveTetromino(0, 1)) {
            lockTetromino();
        }
        lastFallTime = currentTime;
    }
}

void Game::render(SDL_Renderer* renderer, int cellSize) {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int boardRenderWidth = Board::WIDTH * cellSize;
    int boardRenderHeight = Board::HEIGHT * cellSize;

    int offsetX = (windowWidth - boardRenderWidth) / 2; // Center horizontally
    int offsetY = (windowHeight - boardRenderHeight) / 2; // Center vertically

    // Render board
    for (int y = 0; y < Board::HEIGHT; ++y) {
        for (int x = 0; x < Board::WIDTH; ++x) {
            SDL_Color color = board.getGrid()[y][x];
            if (color.a != 0) { // Only draw if not transparent
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect fillRect = {offsetX + (int)(x * cellSize), offsetY + (int)(y * cellSize), cellSize, cellSize};
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }

    // Draw border around the game area
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
    SDL_Rect borderRect = {offsetX, offsetY, boardRenderWidth, boardRenderHeight};
    SDL_RenderDrawRect(renderer, &borderRect);

    // Render ghost piece
    if (!gameOver && !paused) {
        Tetromino ghostTetromino = currentTetromino;
        while (true) {
            Tetromino tempGhost = ghostTetromino;
            tempGhost.move(0, 1);
            if (board.isCollision(tempGhost)) {
                break;
            }
            ghostTetromino.move(0, 1);
        }

        const auto& ghostShape = ghostTetromino.getShape();
        SDL_Color ghostColor = {100, 100, 100, 100}; // Gray, semi-transparent

        for (size_t y = 0; y < ghostShape.size(); ++y) {
            for (size_t x = 0; x < ghostShape[y].size(); ++x) {
                if (ghostShape[y][x] != 0) {
                    SDL_SetRenderDrawColor(renderer, ghostColor.r, ghostColor.g, ghostColor.b, ghostColor.a);
                    SDL_Rect fillRect = {offsetX + (int)((ghostTetromino.getX() + x) * cellSize), offsetY + (int)((ghostTetromino.getY() + y) * cellSize), cellSize, cellSize};
                    SDL_RenderFillRect(renderer, &fillRect);
                }
            }
        }
    }

    // Render current tetromino
    if (!gameOver && !paused) {
        const auto& shape = currentTetromino.getShape();
        SDL_Color color = currentTetromino.getColor();
        int tetroX = currentTetromino.getX();
        int tetroY = currentTetromino.getY();

        for (size_t y = 0; y < shape.size(); ++y) {
            for (size_t x = 0; x < shape[y].size(); ++x) {
                if (shape[y][x] != 0) {
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                    SDL_Rect fillRect = {offsetX + (int)((tetroX + x) * cellSize), offsetY + (int)((tetroY + y) * cellSize), cellSize, cellSize};
                    SDL_RenderFillRect(renderer, &fillRect);
                }
            }
        }
    }

    // Render score
    renderText(renderer, "Score: " + std::to_string(score), offsetX + boardRenderWidth + 10, offsetY + 10, {255, 255, 255, 255});

    // Render next tetromino
    renderNextTetromino(renderer, cellSize, offsetX + boardRenderWidth + 10, offsetY + 100);

    // Render held tetromino
    renderHeldTetromino(renderer, cellSize, offsetX - 100, offsetY + 100); // Position to the left of the board

    // Render game over screen
    if (gameOver) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Semi-transparent black overlay
        SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
        SDL_RenderFillRect(renderer, &overlay);

        renderText(renderer, "GAME OVER", (windowWidth / 2) - 70, (windowHeight / 2) - 30, {255, 0, 0, 255});
        renderText(renderer, "Score: " + std::to_string(score), (windowWidth / 2) - 60, (windowHeight / 2) + 10, {255, 255, 255, 255});
    }

    // Render pause screen
    if (paused && !gameOver) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Semi-transparent black overlay
        SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
        SDL_RenderFillRect(renderer, &overlay);

        renderText(renderer, "PAUSED", (windowWidth / 2) - 50, (windowHeight / 2) - 20, {255, 255, 255, 255});
    }
}

bool Game::moveTetromino(int dx, int dy) {
    Tetromino tempTetromino = currentTetromino; // Create a copy
    tempTetromino.move(dx, dy); // Move the copy
    if (board.isCollision(tempTetromino)) {
        return false; // Collision, so don't move
    }
    currentTetromino.move(dx, dy); // No collision, apply move to actual tetromino
    return true;
}

void Game::rotateTetromino() {
    Tetromino tempTetromino = currentTetromino; // Create a copy
    tempTetromino.rotate(); // Rotate the copy
    if (board.isCollision(tempTetromino)) {
        // Simple wall kick for now (needs proper implementation for Tetris)
        Tetromino testTetromino = tempTetromino;
        testTetromino.move(1, 0); // Try moving right
        if (!board.isCollision(testTetromino)) {
            currentTetromino.rotate();
            currentTetromino.move(1, 0);
            return;
        }
        testTetromino = tempTetromino;
        testTetromino.move(-1, 0); // Try moving left
        if (!board.isCollision(testTetromino)) {
            currentTetromino.rotate();
            currentTetromino.move(-1, 0);
            return;
        }
        return; // No valid rotation
    }
    currentTetromino.rotate(); // No collision, apply rotation to actual tetromino
}

void Game::lockTetromino() {
    board.addTetromino(currentTetromino);
    int linesCleared = board.clearLines();
    updateScore(linesCleared);
    spawnTetromino();
    checkGameOver();
}

void Game::updateScore(int linesCleared) {
    if (linesCleared > 0) {
        score += linesCleared * 100 * level; // Simple scoring
        if (score / 1000 > level -1) { // Increase level every 1000 points
            level++;
            fallDelay = std::max(100u, fallDelay - 50u); // Increase speed
        }
    }
}

void Game::checkGameOver() {
    // Game over if new tetromino spawns in a collision state
    // This is handled in spawnTetromino()
}

void Game::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    if (font == nullptr) return; // Don't render if font not loaded

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    } else {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture == nullptr) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        } else {
            SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void Game::renderNextTetromino(SDL_Renderer* renderer, int cellSize, int xOffset, int yOffset) {
    const auto& shape = nextTetromino.getShape();
    SDL_Color color = nextTetromino.getColor();

    renderText(renderer, "Next:", xOffset, yOffset - 30, {255, 255, 255, 255});

    for (size_t y = 0; y < shape.size(); ++y) {
        for (size_t x = 0; x < shape[y].size(); ++x) {
            if (shape[y][x] != 0) {
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect fillRect = {xOffset + (int)(x * cellSize / 2), yOffset + (int)(y * cellSize / 2), cellSize / 2, cellSize / 2}; // Render smaller
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
}

void Game::renderHeldTetromino(SDL_Renderer* renderer, int cellSize, int xOffset, int yOffset) {
    renderText(renderer, "Hold:", xOffset, yOffset - 30, {255, 255, 255, 255});

    if (heldTetromino.getType() != TetrominoType::None) {
        const auto& shape = heldTetromino.getShape();
        SDL_Color color = heldTetromino.getColor();

        for (size_t y = 0; y < shape.size(); ++y) {
            for (size_t x = 0; x < shape[y].size(); ++x) {
                if (shape[y][x] != 0) {
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                    SDL_Rect fillRect = {xOffset + (int)(x * cellSize / 2), yOffset + (int)(y * cellSize / 2), cellSize / 2, cellSize / 2}; // Render smaller
                    SDL_RenderFillRect(renderer, &fillRect);
                }
            }
        }
    }
}

void Game::swapTetromino() {
    if (!canSwap) return; // Only allow one swap per piece

    if (heldTetromino.getType() == TetrominoType::None) {
        heldTetromino = currentTetromino;
        spawnTetromino(); // Spawn a new piece
    } else {
        Tetromino temp = currentTetromino;
        currentTetromino = heldTetromino;
        heldTetromino = temp;
        // Reset position of swapped piece to top center
        currentTetromino.move(-currentTetromino.getX(), -currentTetromino.getY()); // Reset to 0,0
        currentTetromino.move(Board::WIDTH / 2 - currentTetromino.getShape()[0].size() / 2, 0);
    }
    canSwap = false;
}
