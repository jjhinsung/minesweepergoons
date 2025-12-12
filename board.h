#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Board {
public:
    Board(int rows, int cols, int mines);

    void draw(sf::RenderWindow &window);
    void reveal(int r, int c);
    void toggleFlag(int r, int c);

private:
    struct Tile {
        bool isBomb = false;
        bool isRevealed = false;
        bool isFlagged = false;
        int number = 0;
        sf::Sprite sprite;
    };

    int rows, cols, mines;
    int tileSize = 32;

    sf::Texture hiddenTex;
    sf::Texture flagTex;
    sf::Texture bombTex;
    sf::Texture emptyTex;
    sf::Texture numberTex[8];

    std::vector<std::vector<Tile>> grid;

    void loadTextures();
    void placeMines();
    void calculateNumbers();
    void floodFill(int r, int c);
};