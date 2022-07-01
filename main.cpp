#include <iostream>
#include "SFML/Graphics.hpp"
#include "constants.h"
#include "board.h"
#include <ctime>

int main() {
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Minesweeper");
    
    sf::Font font;
    if(!font.loadFromFile("font/Pixeled.ttf")) {
        std::cerr << "Could not load font.";
    }
    Board board(8,8, font);
    while(window.isOpen()) {
        sf::Event event;
        if(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                Square& square = board.GetSquareAtWindowPos(mousePos.x, mousePos.y);
                if(!board.IsGameOver()) {
                    if(event.mouseButton.button == sf::Mouse::Right) {
                    board.ToggleFlag(square);
                    }
                    else if(event.mouseButton.button == sf::Mouse::Left) {
                        board.Reveal(square);
                    }
                }  
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && board.IsGameOver()) {
                board.Reset();
            }
        }
        window.clear(sf::Color::White);
        board.Draw(window);
        if(board.IsGameOver()) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(24);
            text.setString("Press Space to Restart!");
            text.setPosition(sf::Vector2f(150, WINDOW_HEIGHT / 2));
            text.setFillColor(sf::Color::Black);
            window.draw(text);
        }
        window.display();
    }
    return 0;
}