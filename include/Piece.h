#pragma once

#include <string>
#include <random>
#include "../utils/Vector.h"

class Piece{
public:

    Piece(); 

    int plusSign[3][3];
    int smallSquare[3][3];
    int cornerPiece[3][3];
    int tallTower[3][3];
    int zigzag[3][3];
    int uShape[3][3];
    int stepShape[3][3];
    int arrowPointingUp[3][3];
    int doubleZigzag[3][3];
    int pyramid[3][3];

    int exclusiveShape1[3][3];
    int exclusiveShape2[3][3];
    int exclusiveShape3[3][3];

    int pieceList[13][3][3];

    int rotationCounter=0;
    int allowedRotationNumber=12;

    int (&chooseAPiece(int& currentPieceIndex, int& tempPieceIndex))[3][3];
    

    template<size_t row, size_t column>
    void rotateShapeRight(int (&arrayMatrix)[row][column]);
    template<size_t row, size_t column>
    void rotateShapeLeft(int (&arrayMatrix)[row][column]);



};

#include "Piece.cpp"
