cmake_minimum_required(VERSION 3.16)
project(minesweepergoons)

set(CMAKE_CXX_STANDARD 17)

# Point CMake to SFML 2 (Homebrew)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")

# Find SFML 2 modules
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

# Include headers
include_directories(/opt/homebrew/opt/sfml@2/include)

add_executable(minesweepergoons
        main.cpp
        board.cpp
        board.cpp
)

# Link SFML 2 libraries
target_link_libraries(minesweepergoons
        sfml-graphics
        sfml-window
        sfml-system
)

