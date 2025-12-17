#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Board
{
public:
    Board(int rows, int cols, int mines);

    void draw(sf::RenderWindow &window);
    void checkWin();
    void reveal(int r, int c);
    void toggleFlag(int r, int c);
    void reset(int newRows, int newCols, int newMines);
    bool getTileFromPixel(int mouseX, int mouseY, int &r, int &c, sf::RenderWindow &window);
    bool gameOver = false;
    bool win = false;
    bool isGameOver() const
    {
        return gameOver;
    }
    bool didWin() const
    {
        return win;
    }


private:
    struct Tile
    {
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