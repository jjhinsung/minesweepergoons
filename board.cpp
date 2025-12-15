#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines)
    : rows(rows), cols(cols), mines(mines)
{
    grid.resize(rows, std::vector<Tile>(cols));
    loadTextures();
    placeMines();
    calculateNumbers();
}

void Board::loadTextures() {
    hiddenTex.loadFromFile("assets/hidden.png");
    flagTex.loadFromFile("assets/flag.png");
    bombTex.loadFromFile("assets/bomb.png");
    emptyTex.loadFromFile("assets/empty.png");

    for (int i = 0; i < 8; i++) {
        numberTex[i].loadFromFile("assets/" + std::to_string(i+1) + ".png");
    }
}

void Board::placeMines() {
    srand(time(nullptr));
    int placed = 0;
    while (placed < mines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!grid[r][c].isBomb) {
            grid[r][c].isBomb = true;
            placed++;
        }
    }
}

void Board::calculateNumbers() {
    int dr[] = {-1,-1,-1,0,0,1,1,1};
    int dc[] = {-1,0,1,-1,1,-1,0,1};

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c].isBomb) continue;

            int count = 0;
            for (int i = 0; i < 8; i++) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    if (grid[nr][nc].isBomb) count++;
                }
            }

            grid[r][c].number = count;
        }
    }
}


void Board::draw(sf::RenderWindow &window) {
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(tileSize, tileSize));

    float boardWidth  = cols * tileSize;
    float boardHeight = rows * tileSize;

    float offsetX = (window.getSize().x - boardWidth) / 2.f;
    float offsetY = (window.getSize().y - boardHeight) / 2.f;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Tile &t = grid[r][c];

            // 🎨 Decide color based on state
            if (!t.isRevealed) {
                rect.setFillColor(
                    t.isFlagged ? sf::Color::Red : sf::Color(80, 80, 80)
                );
            } else {
                if (t.isBomb)
                    rect.setFillColor(sf::Color::Black);
                else
                    rect.setFillColor(sf::Color(160, 160, 160));
            }

            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color::Black);

            rect.setPosition(
                offsetX + c * tileSize,
                offsetY + r * tileSize
            );

            window.draw(rect);
        }
    }
}

void Board::reveal(int r, int c) {}
void Board::toggleFlag(int r, int c) {}
void Board::floodFill(int r, int c) {}
