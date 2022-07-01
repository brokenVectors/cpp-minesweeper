#include "board.h"
#include "constants.h"
#include <vector>
#include <math.h>
#include <string> 
#include <iostream>
#include <ctime>
#include "SFML/Graphics.hpp"
Board::Board(int rows, int columns, sf::Font font) {
    Rows = rows;
    Font = font;
    Columns = columns;
    GameOver = false;
    GameWon = false;
    for (int i = 0; i < rows; i++) {
        Grid.push_back(std::vector<Square>());
        SpriteGrid.push_back(std::vector<sf::RectangleShape>());
        for (int j = 0; j < columns; j++)
        {
            Grid[i].push_back(Square {empty, -1, false, i, j});
            SpriteGrid[i].push_back(sf::RectangleShape());
        }   
    }
    Reset();
}
void Board::Reset() {
    GameOver = false;
    InitializeBombs(0.2);
    InitializeBombCounts();
}
Square& Board::GetSquareAt(int x, int y) {
    return Grid[x][y];
}
void Board::ToggleFlag(Square& square) {
    std::cout << (square.type == flagged) << std::endl;
    if(square.type != revealed && square.type != flagged) {
        square.type = flagged;
    } 
    else if(square.type == flagged) {
        std::cout << "unflag" << std::endl;
        square.type = empty;
    }
}
void Board::Reveal(Square& square) {
    if(square.type == empty) {
        square.type = revealed;
        Grid[square.x][square.y] = square; // This is unbelievably hacky and I do not know how to fix it
        if(square.bombsAround == 0 && square.isBomb == false) {
            std::vector<Square> adjacentSquares = GetSquaresAdjacentTo(square);
            for(int i = 0; i < adjacentSquares.size(); i++) {
                if(adjacentSquares[i].type == empty) {
                    Reveal(adjacentSquares[i]);
                }
            }   
        }
        if(square.isBomb) {
            GameOver = true;
        }
    } 
}
Square& Board::GetSquareAtWindowPos(int window_x, int window_y) {
    float x = (window_x - 20) / (WINDOW_WIDTH / Rows - 5);
    float y = (window_y - 30) / (WINDOW_HEIGHT / Columns - 5);
    return GetSquareAt(floor(x), floor(y));
}
void Board::Draw(sf::RenderWindow& window) {
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            sf::RectangleShape& sprite{SpriteGrid[i][j]};
            Square& square{Grid[i][j]};
            sf::Vector2f pos = sf::Vector2f(5 + WINDOW_WIDTH / Rows * i, 5 + WINDOW_HEIGHT / Columns * j);
            sf::Vector2f size = sf::Vector2f(WINDOW_WIDTH / Rows - 5, WINDOW_HEIGHT / Columns - 5);
            if(square.type == empty) sprite.setFillColor(sf::Color(230,230,230));
            if(square.type == flagged) sprite.setFillColor(sf::Color(255, 153, 153));
            if(IsGameOver()) {
                if(square.isBomb) sprite.setFillColor(sf::Color(80, 80, 80));
            }
            else {
                if(square.isBomb && square.type == revealed) sprite.setFillColor(sf::Color(80, 80, 80));
            }
            
            
            sprite.setSize(size);
            sprite.setPosition(pos);
            window.draw(sprite);

            if(square.type == revealed && square.isBomb == false) {
                if(square.bombsAround > 0) {
                    sf::Text text;
                    text.setFont(Font);
                    text.setCharacterSize(24);
                    text.setFillColor(numPalette[square.bombsAround-1]);
                    text.setOrigin(sf::Vector2f(12.f, 12.f));
                    text.setPosition(pos + sf::Vector2f(size.x / 2, size.y / 2));
                    text.setString(std::to_string(square.bombsAround));
                    window.draw(text);
                }
                else {
                    sprite.setFillColor(sf::Color(200,200,200));
                }
            }
            
        }
    }
}
void Board::InitializeBombs(float probability) {
    for(int i = 0; i < Rows; i++) {
        for(int j = 0; j < Columns; j++) {
            bool isBomb = std::rand() < (RAND_MAX * probability);
            Square& square = GetSquareAt(i, j);
            square.isBomb = isBomb;
        }
    }
}
std::vector<Square> Board::GetSquaresAdjacentTo(Square& square) {
    std::vector<Square> adjacentSquares;
    int i = square.x;
    int j = square.y;
    if(i > 0) adjacentSquares.push_back(GetSquareAt(i-1, j));
    if(j > 0) adjacentSquares.push_back(GetSquareAt(i, j-1));
    if(i < 7) adjacentSquares.push_back(GetSquareAt(i+1, j));
    if(j < 7) adjacentSquares.push_back(GetSquareAt(i, j+1));
    if(i > 0 && j > 0) adjacentSquares.push_back(GetSquareAt(i-1, j-1));
    if(i < 7 && j < 7) adjacentSquares.push_back(GetSquareAt(i+1, j+1));
    if(i < 7 && j > 0) adjacentSquares.push_back(GetSquareAt(i+1, j-1));
    if(i > 0 && j < 7) adjacentSquares.push_back(GetSquareAt(i-1, j+1));
    return adjacentSquares;
}
void Board::InitializeBombCounts() {
    for(int i = 0; i < Rows; i++) {
        for(int j = 0; j < Columns; j++) {
            int bombCount = 0;
            Square& square = GetSquareAt(i, j);
            std::vector<Square> adjacentSquares = GetSquaresAdjacentTo(square);
            for(int i = 0; i < adjacentSquares.size(); i++) {
                if(adjacentSquares[i].isBomb) {
                    ++bombCount;
                }
            }
            square.type = empty;
            square.bombsAround = bombCount;
        }
    }
}
bool Board::IsGameOver() {
    return GameOver;
}
bool Board::IsGameWon() {
    if(IsGameOver()) {
        return false;
    }
    int unrevealedSquares = 0;
    int bombs = 0;
    for(int i = 0; i < Rows; i++) {
        for(int j = 0; j < Columns; j++) {
            Square& square = GetSquareAt(i,j);
            if(square.type != revealed) ++unrevealedSquares;
            if(square.isBomb) ++ bombs;
        }
    }
    return unrevealedSquares == bombs;
}