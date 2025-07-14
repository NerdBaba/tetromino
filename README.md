# Tetromino Game Engine

A simple Tetris-like game engine implemented in C++ using SDL2.

## Features
- Core Tetris gameplay mechanics (tetromino movement, rotation, line clearing)
- Dynamic scaling of the game area to fit the window
- Centered game board with a visual border
- Ghost piece to show where the tetromino will land
- Hold functionality to swap pieces
- Pause menu
- Basic scoring system
- Sound effects and background music

## Building and Running

### Prerequisites
- CMake (version 3.10 or higher)
- SDL2 development libraries (`libsdl2-dev`)
- SDL2_ttf development libraries (`libsdl2-ttf-dev`)
- SDL2_mixer development libraries (`libsdl2-mixer-dev`)

On Debian/Ubuntu, you can install these with:
```bash
sudo apt-get update
sudo apt-get install cmake libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Compilation
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd tetromino
   ```
2. Create a `build` directory and compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Running the Game
From the `build` directory:
```bash
./TetrisEngine
```

You can also specify window dimensions:
```bash
./TetrisEngine <width> <height>
```

## Assets
Sound files (`.mp3`) are located in the `assets/sounds/` directory.
Font file (`.otf`) is located in the `build/` directory.

## Controls
- **Left/Right Arrow Keys**: Move tetromino
- **Down Arrow Key**: Soft drop
- **Up Arrow Key**: Rotate tetromino
- **Spacebar**: Hard drop
- **Return Key**: Swap current tetromino with held tetromino
- **Escape Key**: Toggle pause menu
