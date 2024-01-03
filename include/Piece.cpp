#include "Piece.h"


Piece::Piece(): 
    plusSign{{0, 1, 0}, {1, 1, 1}, {0, 1, 0}},
    smallSquare{{1, 1, 0}, {1, 1, 0},{0, 0, 0}},
    cornerPiece{{0, 1, 0}, {0, 1, 1},{0, 0, 0}},
    tallTower{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    zigzag{{0, 1, 1}, {1, 1, 0},{0, 0, 0}},
    uShape{{1, 0, 1}, {1, 1, 1},{0, 0, 0}},
    stepShape{{1, 0, 0}, {1, 1, 0}, {0, 1, 0}},
    arrowPointingUp{{0, 1, 0}, {1, 1, 1}, {1, 0, 1}},
    doubleZigzag{{1, 0, 0}, {1, 1, 1}, {0, 0, 1}},
    pyramid{{0, 1, 0}, {1, 1, 1},{0, 0, 0}},
    exclusiveShape1{{1, 1, 0}, {1, 1, 0},{0, 0, 0}},
    exclusiveShape2{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    exclusiveShape3{{0, 1, 0}, {1, 1, 1},{0, 0, 0}}
    
    {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                pieceList[0][i][j] = plusSign[i][j];
                pieceList[1][i][j] = smallSquare[i][j];
                pieceList[2][i][j] = cornerPiece[i][j];
                pieceList[3][i][j] = tallTower[i][j];
                pieceList[4][i][j] = zigzag[i][j];
                pieceList[5][i][j] = uShape[i][j];
                pieceList[6][i][j] = stepShape[i][j];
                pieceList[7][i][j] = arrowPointingUp[i][j];
                pieceList[8][i][j] = doubleZigzag[i][j];
                pieceList[9][i][j] = pyramid[i][j];
                pieceList[10][i][j] = exclusiveShape1[i][j];
                pieceList[11][i][j] = exclusiveShape2[i][j];
                pieceList[12][i][j] = exclusiveShape3[i][j];
            }
        }
    }

int (&Piece::chooseAPiece(int& currentPieceIndex, int& tempPieceIndex))[3][3]{

    //to make the game easy to play added weight for each piece
    Vector<int> indexes={0,1,2,3,4,5,6,7,8,9,10,11,12};
    Vector<int> weights={1,5,5,5,5,3,3,2,2,5,3,3,3};

    std::random_device rd;
    std::mt19937 generator(rd());

    // Define a distributio
    std::discrete_distribution<> distribution(weights.begin(), weights.end());

    // Generate a random integer
    int randomNumber;
    tempPieceIndex=currentPieceIndex;
    while(true){        
        randomNumber = indexes[distribution(generator)];
        currentPieceIndex=randomNumber;
        if(currentPieceIndex==tempPieceIndex){
            continue;
        }else{
            break;
        }
    }

    return pieceList[randomNumber];
}

template<size_t row, size_t column>
void Piece::rotateShapeRight(int (&arrayMatrix)[row][column]){
    int copyMatrix[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            copyMatrix[i][j]=arrayMatrix[i][j];
        }
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            int newI=i-1;
            int newJ=j-1;
            int tempI=newI;
            newI=newJ+1;
            newJ=(-1*tempI)+1;
            arrayMatrix[newI][newJ]=copyMatrix[i][j];
        }
    }
}

template<size_t row, size_t column>
void Piece::rotateShapeLeft(int (&arrayMatrix)[row][column]){
    int copyMatrix[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            copyMatrix[i][j]=arrayMatrix[i][j];
        }
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            int newI=i-1;
            int newJ=j-1;
            int tempI=newI;
            newI=-1*newJ+1;
            newJ=tempI+1;
            arrayMatrix[newI][newJ]=copyMatrix[i][j];
        }
    }
}