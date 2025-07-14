#include "Tetromino.h"
#include <cstdlib> // For rand()

Tetromino::Tetromino(TetrominoType type) : x(0), y(0) {
    setShapeAndColor(type);
}

void Tetromino::setShapeAndColor(TetrominoType type) {
    this->type = type;
    switch (type) {
        case TetrominoType::I:
            shape = {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            };
            color = {0, 255, 255, 255}; // Cyan
            x = 3; y = 0;
            break;
        case TetrominoType::O:
            shape = {
                {1, 1},
                {1, 1}
            };
            color = {255, 255, 0, 255}; // Yellow
            x = 4; y = 0;
            break;
        case TetrominoType::T:
            shape = {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}
            };
            color = {128, 0, 128, 255}; // Purple
            x = 3; y = 0;
            break;
        case TetrominoType::S:
            shape = {
                {0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}
            };
            color = {0, 255, 0, 255}; // Green
            x = 3; y = 0;
            break;
        case TetrominoType::Z:
            shape = {
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}
            };
            color = {255, 0, 0, 255}; // Red
            x = 3; y = 0;
            break;
        case TetrominoType::J:
            shape = {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}
            };
            color = {0, 0, 255, 255}; // Blue
            x = 3; y = 0;
            break;
        case TetrominoType::L:
            shape = {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}
            };
            color = {255, 165, 0, 255}; // Orange
            x = 3; y = 0;
            break;
        case TetrominoType::None:
            shape = {};
            color = {0, 0, 0, 0};
            break;
    }
}

void Tetromino::rotate() {
    if (type == TetrominoType::O) return; // O block doesn't rotate

    std::vector<std::vector<int>> rotatedShape(shape[0].size(), std::vector<int>(shape.size()));
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[0].size(); ++j) {
            rotatedShape[j][shape.size() - 1 - i] = shape[i][j];
        }
    }
    shape = rotatedShape;
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
}
