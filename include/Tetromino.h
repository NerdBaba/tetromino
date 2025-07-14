#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <SDL.h>

enum class TetrominoType {
    I, O, T, S, Z, J, L, None
};

class Tetromino {
public:
    Tetromino(TetrominoType type = TetrominoType::None);

    void rotate();
    void move(int dx, int dy);

    const std::vector<std::vector<int>>& getShape() const { return shape; }
    SDL_Color getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }
    TetrominoType getType() const { return type; }

private:
    TetrominoType type;
    std::vector<std::vector<int>> shape;
    SDL_Color color;
    int x, y; // Position on the game board

    void setShapeAndColor(TetrominoType type);
};

#endif // TETROMINO_H
