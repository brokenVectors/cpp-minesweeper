#include <iostream>
#include "SFML/Graphics.hpp"
#include "constants.h"
#include "board.h"
#include "SFML/Audio.hpp"
#include <ctime>

int main() {
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Minesweeper");
    
    sf::Font font;
    if(!font.loadFromFile("font/Pixeled.ttf")) {
        std::cerr << "Could not load font.";
    }
    sf::SoundBuffer lossBuffer;
    if(!lossBuffer.loadFromFile("sounds/explosion.wav")) return -1;
    sf::Sound lossSound;
    lossSound.setBuffer(lossBuffer);

    sf::SoundBuffer winBuffer;
    if(!winBuffer.loadFromFile("sounds/win.wav")) return -1;
    sf::Sound winSound;
    winSound.setBuffer(winBuffer);

    Board board(8,8, font, lossSound, winSound);
    while(window.isOpen()) {
        sf::Event event;
        if(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                Square& square = board.GetSquareAtWindowPos(mousePos.x, mousePos.y);
                if(!(board.IsGameOver() || board.IsGameWon()) ) {
                    if(event.mouseButton.button == sf::Mouse::Right) {
                    board.ToggleFlag(square);
                    }
                    else if(event.mouseButton.button == sf::Mouse::Left) {
                        board.Reveal(square);
                    }
                }  
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && (board.IsGameOver() || board.IsGameWon())) {
                board.Reset();
            }
        }
        window.clear(sf::Color::White);
        board.Draw(window);
        if(board.IsGameOver() || board.IsGameWon()) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(24);
            text.setPosition(sf::Vector2f(150, WINDOW_HEIGHT / 2));
            text.setFillColor(sf::Color::Black);
            
            if(board.IsGameOver()) {
                text.setString("Press Space to Restart!");
            }
            else {
                text.setString("Nice one!\nPress Space to Restart!");
            }
            window.draw(text);
        }
        window.display();
    }
    return 0;
}