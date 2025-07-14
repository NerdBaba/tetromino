cmake_minimum_required(VERSION 3.10)
project(TetrisEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(include)

add_executable(TetrisEngine src/main.cpp src/Tetromino.cpp src/Board.cpp src/Game.cpp)
target_link_libraries(TetrisEngine SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
