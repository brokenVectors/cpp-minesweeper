#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "SFML/Graphics.hpp"
enum SquareType {
    empty, flagged, revealed
};
struct Square {
    SquareType type;
    int bombsAround;
    bool isBomb;
    int x;
    int y;
};

class Board {
    private:
    std::vector<std::vector<Square>> Grid;
    std::vector<std::vector<sf::RectangleShape>> SpriteGrid;
    sf::Color numPalette[8] {sf::Color(0,0,255), sf::Color(0,255,0), sf::Color(255,0,0), sf::Color(0,0,100), sf::Color(100,0,0), sf::Color(64,224,208), sf::Color(0,0,0), sf::Color(100,100,100)};
    sf::Font Font;
    int Rows;
    int Columns;
    bool GameOver;
    public:
    
    Board(int rows, int columns, sf::Font font);
    Square& GetSquareAt(int x, int y);
    Square& GetSquareAtWindowPos(int window_x, int window_y);
    void ToggleFlag(Square& square);
    void Reveal(Square& square);
    void Draw(sf::RenderWindow& window);
    void InitializeBombs(float probability);
    void InitializeBombCounts();
    bool IsGameOver();
    void Reset();
    std::vector<Square> GetSquaresAdjacentTo(Square& square);
};
#endif