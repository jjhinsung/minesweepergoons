#include <SFML/Graphics.hpp>
#include "board.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "Minesweeper");

    Board board(10, 10, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            int row, col;
            if (board.isGameOver())
                continue;
            bool valid = board.getTileFromPixel(
                event.mouseButton.x,
                event.mouseButton.y,
                row,
                col,
                window
            );

            if (!valid) continue;

            // LEFT CLICK → reveal
            if (event.mouseButton.button == sf::Mouse::Left) {
                board.reveal(row, col);
            }

            // RIGHT CLICK → flag
            if (event.mouseButton.button == sf::Mouse::Right) {
                board.toggleFlag(row, col);
            }
        }

        window.clear();
        board.draw(window);
        if (board.isGameOver()) {
            sf::Font font;
            font.loadFromFile("assets/SuperJoyful-lxwPq.ttf");

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::White);

            if (board.didWin())
                text.setString("YOU WIN");
            else
                text.setString("GAME OVER");

            text.setPosition(200, 200); // simple center-ish
            window.draw(text);
        }
        window.display();


    }

    return 0;
}