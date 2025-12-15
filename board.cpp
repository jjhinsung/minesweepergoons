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
    float boardWidth  = cols * tileSize;
    float boardHeight = rows * tileSize;

    float offsetX = (window.getSize().x - boardWidth) / 2.f;
    float offsetY = (window.getSize().y - boardHeight) / 2.f;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Tile &t = grid[r][c];

            // 🧠 choose texture based on tile state
            if (!t.isRevealed) {
                if (t.isFlagged)
                    t.sprite.setTexture(flagTex);
                else
                    t.sprite.setTexture(hiddenTex);
            } else {
                if (t.isBomb)
                    t.sprite.setTexture(bombTex);
                else if (t.number == 0)
                    t.sprite.setTexture(emptyTex);
                else
                    t.sprite.setTexture(numberTex[t.number - 1]);
            }

            // 📍 position sprite (centered board)
            t.sprite.setPosition(
                offsetX + c * tileSize,
                offsetY + r * tileSize
            );

            window.draw(t.sprite);
        }
    }
}

void Board::reveal(int r, int c) {}
void Board::toggleFlag(int r, int c) {}
void Board::floodFill(int r, int c) {}
