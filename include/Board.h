#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <SDL.h>
#include "Tetromino.h"

class Board {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;

    Board();

    bool isCollision(const Tetromino& tetromino) const;
    void addTetromino(const Tetromino& tetromino);
    int clearLines(); // Returns number of lines cleared
    void reset();

    const std::vector<std::vector<SDL_Color>>& getGrid() const { return grid; }

private:
    std::vector<std::vector<SDL_Color>> grid;
};

#endif // BOARD_H
