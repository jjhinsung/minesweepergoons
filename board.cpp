#include "board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines)
    : rows(rows), cols(cols), mines(mines)
{
    grid.resize(rows, std::vector<Tile>(cols));
    loadTextures();
    placeMines();
    calculateNumbers();
    // TEMPORARY TEST
    /*for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            reveal(row, col);
        }
    }*/

}

bool Board::getTileFromPixel(int mouseX, int mouseY, int &r, int &c, sf::RenderWindow &window) {
    float boardWidth  = cols * tileSize;
    float boardHeight = rows * tileSize;

    float offsetX = (window.getSize().x - boardWidth) / 2.f;
    float offsetY = (window.getSize().y - boardHeight) / 2.f;

    // Convert pixel → grid coordinates
    c = (mouseX - offsetX) / tileSize;
    r = (mouseY - offsetY) / tileSize;

    // Check bounds
    if (r < 0 || r >= rows || c < 0 || c >= cols)
        return false;

    return true;
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

void Board::checkWin() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Tile &t = grid[r][c];
            if (!t.isBomb && !t.isRevealed)
                return;
        }
    }

    // all safe tiles revealed
    gameOver = true;
    win = true;
}

    void Board::reveal(int r, int c) {
        // bounds check
        if (r < 0 || r >= rows || c < 0 || c >= cols)
            return;

        Tile &t = grid[r][c];

        // do nothing if already revealed or flagged
        if (t.isRevealed || t.isFlagged)
            return;

        // reveal this tile
        t.isRevealed = true;

        // if empty tile, expand
        if (!t.isBomb && t.number == 0) {
            floodFill(r, c);
        }

        // 💥 LOSE CONDITION
        if (t.isBomb) {
            gameOver = true;
            win = false;

            // reveal all bombs
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (grid[i][j].isBomb)
                        grid[i][j].isRevealed = true;
                }
            }
            return;

        }
        // 🏆 check win after each reveal
        checkWin();
    }

    void Board::toggleFlag(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols)
            return;

        Tile &t = grid[r][c];

        // don't allow flags on revealed tiles
        if (t.isRevealed)
            return;

        t.isFlagged = !t.isFlagged;
    }


    void Board::floodFill(int r, int c) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr;
                int nc = c + dc;

                // skip center tile
                if (dr == 0 && dc == 0)
                    continue;

                // bounds check
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols)
                    continue;

                Tile &neighbor = grid[nr][nc];

                // skip revealed or flagged tiles
                if (neighbor.isRevealed || neighbor.isFlagged)
                    continue;

                neighbor.isRevealed = true;

                // recurse if neighbor is also empty
                if (!neighbor.isBomb && neighbor.number == 0) {
                    floodFill(nr, nc);
                }
            }
        }
    }
}

void Board::reset(int newRows, int newCols, int newMines) {
    rows  = newRows;
    cols  = newCols;
    mines = newMines;

    // rebuild grid (use std::vector consistently)
    grid.clear();
    grid.resize(rows, std::vector<Tile>(cols));

    // reset tile state (optional because default Tile values already do this,
    // but it's fine to keep)
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid[r][c].isBomb = false;
            grid[r][c].isRevealed = false;
            grid[r][c].isFlagged = false;
            grid[r][c].number = 0;
        }
    }

    placeMines();
    calculateNumbers();
}

    }
