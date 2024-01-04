#pragma once

#include "../utils/Vector.h"
#include "../utils/Stack.h"
#include "../utils/utils.h"
#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility> // for std::pair
#include <thread>
#include <chrono>


class Board{
private:
    float cellSize=30.0;
    size_t rowSize=20;
    size_t columnSize=10;
    sf::Color backgroundColor=sf::Color::White;
    sf::Color outlineColor=sf::Color::Black;
    sf::Color pieceColor=sf::Color::Blue;
public:
    Board();
    Vector<int> gameBoardMatrix;
    void changeCellSize(float cellSize);
    void changeColumnAndRowSize(size_t row,size_t column);
    void updateGameBoard(size_t row,size_t column);
    float getCellSize() const;
    size_t getRowSize() const;
    void setRowSize(size_t newRowSize);
    size_t getColumnSize() const;
    sf::Color getBackgroundColor();
    void setBackgroundColor(sf::Color newColor, bool& isThemeSelectionSelected);
    sf::Color getOutlineColor();
    void setOutlineColor(sf::Color newColor, bool& isThemeSelectionSelected);
    sf::Color getPieceColor();
    void setPieceColor(sf::Color newColor);
    void printBoard();

    //color datas
    Vector<sf::Color> colors={sf::Color::Red, sf::Color::Blue,sf::Color::Yellow,sf::Color::Green,sf::Color::Magenta,sf::Color::Cyan,sf::Color::Black,sf::Color::White};


    //delete a piece from gameBoard
    void deletePiece(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn);
    //this function pu chosen piece to the board. If it can not the game is over.
    bool putAPieceToTheGameBoard(int (&pieceMatrix)[3][3]);
    //this function control wheter the piece can move. if it can it moves and return true if not return false.
    bool CanPieceMove(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn,int newStartingRow, int newStartingColumn);
    //move the piece
    void movePiece(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn, bool canItMove,int newStartingRow, int newStartingColumn);
    //move piece to right
    void movePieceToRight(int (&pieceMatrix)[3][3], int &startingRow,int &startingColumn);
    //move piece to left
    void movePieceToLeft(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn);
    //move piece to downward
    void movePieceToDownward(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn);
    //rotate piece clokwise
    void rotateClockwise(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn, Piece piece, int& allowedRotationNumber, int& rotationCounter, bool& playerCanRotate);
    //rotate piece counterclockwise
    void rotateCounterclockwise(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn, Piece piece, int& allowedRotationNumber, int& rotationCounter,  bool& playerCanRotate);
    
    
    //draw piece preview area and next piece in it
    void drawNextPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&nextPiece)[3][3], sf::Font& font);
    //hold the piece
    void holdPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&currentPiece)[3][3], sf::Font& font, bool& isHoldedAreaEmpty, int &startingRow, int &startingColumn, Piece piece,  int (&holdedPiece)[3][3],int& currentPieceIndex, int& tempPieceIndex);
    //get holded Piece
    void drawHoldPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&nextPiece)[3][3], sf::Font& font);
    //draw score
    void drawScore(int& score, sf::RenderWindow& window, int cellSize, sf::Font& font);
    //increase score
    void increaseScore(int& score);

    //control if a row of gameboard matrix is full of 1
    bool controlRowOfGameBoardMatrix( int& score, int scoreRise);


    //menu functions
    void pauseGame(bool& isPaused, bool& isGameStarted);
    void menuResume(bool& isPaused);
    void menuNewGame(bool& isPaused, bool& isGameStarted, int (&holdedPiece)[3][3], int& score, int& startinRow, int& startingColumn, bool& isNewGameSelected, bool& isHoldedAreaEmpty);
    void menuThemeSelection(bool& isThemeSelectionSelected);


    //theme functions
    void darkTheme();
    void lightTheme();
    void themeGoBackArrow(bool& isThemeSelectionSelected);


    //new game functions
    void newGameGoBack(bool& isNewGameSelected);
    //custom Piece function
    void adjustCustomPiece(int (&customPiece)[3][3], int rowIndex, int columnIndex, sf::RectangleShape& customPieceCell, sf::RenderWindow& window, int positionX, int positionY, Piece& piece, int customPieceIndex);
    //difficulty functions
    void easy(Piece& piece, sf::Window& window,float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit);
    void medium(Piece& piece, sf::Window& window,float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit);
    void hard(Piece& piece, sf::Window& window,float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit);


    //portal functions
    std::pair<int, int> findAPortalPlace();



    //user ınput handling
    //when game over write user name and its score to the file
    void writeUserToFile(std::string userName, int score);
    //findUserIfExistInDataBasa
    bool findAndUpdateUser(std::string playerName, int score);
    //sort scoreBoard using Stack
    void sortScoreBoard();
    void printScoreBoard(sf::Font& font,  int cellSize, sf::RenderWindow& window);
    

    //************************************************************************************
    //shrinking board functions
    bool controlIfEnoughRowAreEmpty();
    void movePiecesToUpShrinkingBoard();
    void movePiecesToDownShrinkingBoard();



    //sounds
    void restartBackgroundMusic(sf::Music& backgroundMusic);
};

#include "Board.cpp"
