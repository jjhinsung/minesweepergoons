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

        window.clear();
        board.draw(window);
        window.display();
    }

    return 0;
}