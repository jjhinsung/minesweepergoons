#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"

using namespace std;

static int clampInt(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

struct Settings {
    int size = 10;   // square: size x size
    int bombs = 10;
    int minSize = 5;
    int maxSize = 30;
};

struct Button {
    sf::RectangleShape box;
    sf::Text label;
    bool contains(sf::Vector2f p) const { return box.getGlobalBounds().contains(p); }
};

static Button makeButton(const sf::Font& font, const string& text, float x, float y, float w, float h) {
    Button b;
    b.box.setPosition(x, y);
    b.box.setSize(sf::Vector2f(w, h));
    b.box.setFillColor(sf::Color(60, 60, 60));
    b.box.setOutlineThickness(2.f);
    b.box.setOutlineColor(sf::Color::White);

    b.label.setFont(font);
    b.label.setString(text);
    b.label.setCharacterSize(18);
    b.label.setFillColor(sf::Color::White);

    sf::FloatRect tb = b.label.getLocalBounds();
    b.label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    b.label.setPosition(x + w / 2.f, y + h / 2.f);

    return b;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Minesweeper");
    window.setFramerateLimit(60);

    // UI fonts (load ONCE)
    sf::Font uiFont;
    if (!uiFont.loadFromFile("assets/arial.ttf")) {
        cout << "Missing font: assets/arial.ttf\n";
        return 1;
    }

    sf::Font endFont;
    if (!endFont.loadFromFile("assets/SuperJoyful-lxwPq.ttf")) {
        cout << "Missing font: assets/SuperJoyful-lxwPq.ttf\n";
        return 1;
    }

    Settings s;
    // Make sure bombs valid for size
    s.bombs = clampInt(s.bombs, 1, s.size * s.size - 1);

    Board board(s.size, s.size, s.bombs);

    bool panelOpen = false;

    // Panel background
    sf::RectangleShape panel(sf::Vector2f(320.f, 260.f));
    panel.setFillColor(sf::Color(20, 20, 20, 230));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color::White);
    panel.setPosition(20.f, 20.f);

    sf::Text title("Settings (P to close)", uiFont, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(35.f, 30.f);

    sf::Text sizeText("", uiFont, 18);
    sizeText.setFillColor(sf::Color::White);
    sizeText.setPosition(35.f, 80.f);

    sf::Text bombsText("", uiFont, 18);
    bombsText.setFillColor(sf::Color::White);
    bombsText.setPosition(35.f, 140.f);

    // Buttons
    Button sizeMinus = makeButton(uiFont, "-", 220.f, 75.f, 40.f, 35.f);
    Button sizePlus  = makeButton(uiFont, "+", 270.f, 75.f, 40.f, 35.f);

    Button bombMinus = makeButton(uiFont, "-", 220.f, 135.f, 40.f, 35.f);
    Button bombPlus  = makeButton(uiFont, "+", 270.f, 135.f, 40.f, 35.f);

    Button applyBtn  = makeButton(uiFont, "Apply", 35.f, 200.f, 130.f, 45.f);
    Button closeBtn  = makeButton(uiFont, "Close", 180.f, 200.f, 130.f, 45.f);

    // End text (create once; update string when needed)
    sf::Text endText("", endFont, 48);
    endText.setFillColor(sf::Color::White);
    endText.setPosition(200.f, 200.f);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                continue;
            }

            // Toggle panel (ONLY ONCE)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                panelOpen = !panelOpen;
                continue;
            }

            // Keyboard changes to settings (optional: apply immediately)
            if (event.type == sf::Event::KeyPressed) {
                bool changed = false;

                if (event.key.code == sf::Keyboard::Up) {
                    s.size = clampInt(s.size + 1, s.minSize, s.maxSize);
                    changed = true;
                } else if (event.key.code == sf::Keyboard::Down) {
                    s.size = clampInt(s.size - 1, s.minSize, s.maxSize);
                    changed = true;
                }

                int maxBombsAllowed = s.size * s.size - 1;
                s.bombs = clampInt(s.bombs, 1, maxBombsAllowed);

                if (event.key.code == sf::Keyboard::Right) {
                    s.bombs = clampInt(s.bombs + 1, 1, maxBombsAllowed);
                    changed = true;
                } else if (event.key.code == sf::Keyboard::Left) {
                    s.bombs = clampInt(s.bombs - 1, 1, maxBombsAllowed);
                    changed = true;
                }

                // Apply instantly only when panel is closed (avoids “UI editing flicker” feeling)
                if (changed && !panelOpen) {
                    board.reset(s.size, s.size, s.bombs);
                }
            }

            // Mouse input
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // If panel is open, handle panel clicks first and DO NOT click the board
                if (panelOpen && event.mouseButton.button == sf::Mouse::Left) {
                    int maxBombsAllowed = s.size * s.size - 1;

                    if (sizeMinus.contains(mouse)) {
                        s.size = clampInt(s.size - 1, s.minSize, s.maxSize);
                        s.bombs = clampInt(s.bombs, 1, s.size * s.size - 1);
                    } else if (sizePlus.contains(mouse)) {
                        s.size = clampInt(s.size + 1, s.minSize, s.maxSize);
                        s.bombs = clampInt(s.bombs, 1, s.size * s.size - 1);
                    } else if (bombMinus.contains(mouse)) {
                        s.bombs = clampInt(s.bombs - 1, 1, maxBombsAllowed);
                    } else if (bombPlus.contains(mouse)) {
                        s.bombs = clampInt(s.bombs + 1, 1, maxBombsAllowed);
                    } else if (applyBtn.contains(mouse)) {
                        board.reset(s.size, s.size, s.bombs);
                    } else if (closeBtn.contains(mouse)) {
                        panelOpen = false;
                    }

                    // Panel consumed the click; do not also click board.
                    continue;
                }

                // If game over, ignore board clicks
                if (board.isGameOver()) {
                    continue;
                }

                // Click board (when panel not consuming input)
                int row, col;
                bool valid = board.getTileFromPixel(
                    event.mouseButton.x,
                    event.mouseButton.y,
                    row,
                    col,
                    window
                );

                if (!valid) continue;

                if (event.mouseButton.button == sf::Mouse::Left) {
                    board.reveal(row, col);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    board.toggleFlag(row, col);
                }
            }
        }

        // Update UI text each frame
        sizeText.setString("Grid Size: " + to_string(s.size) + " x " + to_string(s.size));
        bombsText.setString("Bombs: " + to_string(s.bombs));

        // Draw (ONLY ONCE PER FRAME)
        window.clear(sf::Color::Black);

        board.draw(window);

        if (panelOpen) {
            window.draw(panel);
            window.draw(title);
            window.draw(sizeText);
            window.draw(bombsText);

            window.draw(sizeMinus.box); window.draw(sizeMinus.label);
            window.draw(sizePlus.box);  window.draw(sizePlus.label);
            window.draw(bombMinus.box); window.draw(bombMinus.label);
            window.draw(bombPlus.box);  window.draw(bombPlus.label);

            window.draw(applyBtn.box);  window.draw(applyBtn.label);
            window.draw(closeBtn.box);  window.draw(closeBtn.label);
        }

        if (board.isGameOver()) {
            if (board.didWin()) endText.setString("YOU WIN");
            else               endText.setString("GAME OVER");
            window.draw(endText);
        }

        window.display();
    }

    return 0;
}
