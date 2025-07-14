#include "Board.h"
#include <iostream>

Board::Board() {
    grid.resize(HEIGHT, std::vector<SDL_Color>(WIDTH, {0, 0, 0, 0})); // Initialize with black (empty) cells
}

bool Board::isCollision(const Tetromino& tetromino) const {
    const auto& shape = tetromino.getShape();
    int tetroX = tetromino.getX();
    int tetroY = tetromino.getY();

    for (size_t y = 0; y < shape.size(); ++y) {
        for (size_t x = 0; x < shape[y].size(); ++x) {
            if (shape[y][x] != 0) { // If it's a block in the tetromino
                int boardX = tetroX + x;
                int boardY = tetroY + y;

                // Check boundaries
                if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT) {
                    return true; // Collision with wall or bottom
                }
                // Check collision with existing blocks on the grid (ignore top boundary for initial spawn)
                if (boardY >= 0 && grid[boardY][boardX].a != 0) { // If cell is not transparent (occupied)
                    return true; // Collision with existing block
                }
            }
        }
    }
    return false;
}

void Board::addTetromino(const Tetromino& tetromino) {
    const auto& shape = tetromino.getShape();
    int tetroX = tetromino.getX();
    int tetroY = tetromino.getY();
    SDL_Color color = tetromino.getColor();

    for (size_t y = 0; y < shape.size(); ++y) {
        for (size_t x = 0; x < shape[y].size(); ++x) {
            if (shape[y][x] != 0) {
                if (tetroY + y >= 0) { // Only add if within board boundaries (ignore negative y for initial spawn)
                    grid[tetroY + y][tetroX + x] = color;
                }
            }
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;
    for (int y = HEIGHT - 1; y >= 0; --y) {
        bool lineFull = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x].a == 0) { // If any cell is transparent (empty)
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            linesCleared++;
            for (int rowToMove = y; rowToMove > 0; --rowToMove) {
                for (int x = 0; x < WIDTH; ++x) {
                    grid[rowToMove][x] = grid[rowToMove - 1][x];
                }
            }
            // Clear the top row
            for (int x = 0; x < WIDTH; ++x) {
                grid[0][x] = {0, 0, 0, 0};
            }
            y++; // Recheck the same row as rows have shifted down
        }
    }
    return linesCleared;
}

void Board::reset() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = {0, 0, 0, 0};
        }
    }
}
