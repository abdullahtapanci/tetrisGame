#include "Board.h"

Board::Board(){
    updateGameBoard(rowSize,columnSize);
}

void Board::changeCellSize(float cellSize){
    cellSize=cellSize;
}

void Board::changeColumnAndRowSize(size_t row,size_t column){
    rowSize=row;
    columnSize=column;
    updateGameBoard(row,column);

}
void Board::updateGameBoard(size_t row,size_t column){
    gameBoardMatrix.reserve(row*column);
    gameBoardMatrix.setRowAndColumnSize(row,column);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            gameBoardMatrix.pushBack(0);
        }
    }
}

float Board::getCellSize() const{
    return cellSize;
}
size_t Board::getRowSize() const{
    return rowSize;
}
void Board::setRowSize(size_t newRowSize){
    rowSize=newRowSize;
}
size_t Board::getColumnSize() const{
    return columnSize;
}


sf::Color Board::getBackgroundColor(){
    return backgroundColor;
}
void Board::setBackgroundColor(sf::Color newColor, bool& isThemeSelectionSelected){
    backgroundColor=newColor;
    isThemeSelectionSelected=false;

}
sf::Color Board::getOutlineColor(){
    return outlineColor;
}
void Board::setOutlineColor(sf::Color newColor, bool& isThemeSelectionSelected){
    outlineColor=newColor;
    isThemeSelectionSelected=false;
}
sf::Color Board::getPieceColor(){
    return pieceColor;
}
void Board::setPieceColor(sf::Color newColor){
    pieceColor=newColor;
}





void Board::printBoard(){
    for(int i=0;i<rowSize;i++){
        for(int j=0;j<columnSize;j++){
            std::cout<<gameBoardMatrix(i,j);
        }
        std::cout<<std::endl;
    }
}



void Board::deletePiece(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn){
    int sRow=startingRow, sColumn=startingColumn;
    for(auto &rowElement:pieceMatrix ){
        sColumn=startingColumn;
        for(auto &columnElement:rowElement){
            if(columnElement==1){
                gameBoardMatrix(sRow,sColumn)=0;
            }
            sColumn++;
        }
        sRow++;
    }
}





bool Board::putAPieceToTheGameBoard(int (&pieceMatrix)[3][3]){
    int rowIndex=0,columnIndex;
    for(auto &row:pieceMatrix){
        columnIndex=(getColumnSize()/2)-1;
        for(auto& column:row){
            if(gameBoardMatrix(rowIndex,columnIndex)!=0&&column!=0){
                return false;
            }
            columnIndex++;
        }
        rowIndex++;
    }
    return true;
}




bool Board::CanPieceMove(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn,int newStartingRow, int newStartingColumn){
    //delete the piece from board
    deletePiece(pieceMatrix,startingRow,startingColumn);

    //create a flag that holds boolean value of piece
    bool canItMove=true;
    
    //control if there is a piece that is an obstacle
    int sRow=newStartingRow,sColumn=newStartingColumn;
    for(auto &rowElement:pieceMatrix){
        sColumn=newStartingColumn;
        for(auto &columnElement:rowElement){
            if(columnElement==1){
                if(sRow<=(getRowSize()-1) && sColumn<=(getColumnSize()-1) && sColumn>=0 && gameBoardMatrix(sRow,sColumn)==1){
                    canItMove=false;
                    break;
                }
            }
            sColumn++;
        }
        if(canItMove==0)
            break;
        sRow++;
    }
    //control if piece reach at the bottom of the gameBoard
    sRow=newStartingRow;
    sColumn=newStartingColumn;
    //check if sRow+2 equals to gameBoard row size and third column of piece matrix is full of zero and update gameBoard accordingly
    if( sRow+2 == (getRowSize()) && (pieceMatrix[2][0]==0 && pieceMatrix[2][1]==0 && pieceMatrix[2][2]==0) && canItMove){
        canItMove=true;
    }else if(sRow+2 == (getRowSize()-1) && (pieceMatrix[2][0]==1 || pieceMatrix[2][1]==1 || pieceMatrix[2][2]==1) && canItMove){
        canItMove=true;
    }else if(sRow+2 > (getRowSize()-1)){
        canItMove=false;
    }
    
    return canItMove;
}



void Board::movePiece(int (&pieceMatrix)[3][3],int &startingRow,int &startingColumn,bool canItMove,int newStartingRow, int newStartingColumn){
    //if it can not move, leave it where it was
    int sRow,sColumn;
    bool isPieceDeleted=false;
    if(canItMove==0){
        sRow=startingRow,sColumn=startingColumn;
        for(auto &rowElement:pieceMatrix){
            sColumn=startingColumn;
            for(auto &columnElement:rowElement){
                if(columnElement==1){
                    gameBoardMatrix(sRow,sColumn)=1;
                }
                sColumn++;
            }
            sRow++;
        }
    }else{
        //if it can move, update gameBoard accordingly
        startingRow=newStartingRow;
        startingColumn=newStartingColumn;
        sRow=startingRow;
        sColumn=startingColumn;

        for(auto &rowElement:pieceMatrix){
            sColumn=startingColumn;
            for(auto &columnElement:rowElement){
                if(columnElement==1){
                    gameBoardMatrix(sRow,sColumn)=1;
                }
                sColumn++;
            }
            if(isPieceDeleted)
                break;
            sRow++;
        }
    }
}




void Board::movePieceToRight(int (&pieceMatrix)[3][3], int &startingRow,int &startingColumn){
    //delete current piece
    deletePiece(pieceMatrix,startingRow,startingColumn);
    if( ( (startingColumn+3)<=(getColumnSize()-1) ) || ( (startingColumn+2)==(getColumnSize()-1) && (pieceMatrix[0][2]==0 && pieceMatrix[1][2]==0 && pieceMatrix[2][2]==0) ) ){
        bool canItMove=CanPieceMove(pieceMatrix,startingRow,startingColumn,startingRow,startingColumn+1);
        movePiece(pieceMatrix,startingRow,startingColumn,canItMove,startingRow,startingColumn+1);
    }
}




void Board::movePieceToLeft(int (&pieceMatrix)[3][3], int &startingRow,int &startingColumn){
    //delete current piece
    deletePiece(pieceMatrix,startingRow,startingColumn);
    if( ( (startingColumn-1)>=0) || ( (startingColumn)==0 && (pieceMatrix[0][0]==0 && pieceMatrix[1][0]==0 && pieceMatrix[2][0]==0) ) ){
        bool canItMove=CanPieceMove(pieceMatrix,startingRow,startingColumn,startingRow,startingColumn-1);
        movePiece(pieceMatrix,startingRow,startingColumn,canItMove,startingRow,startingColumn-1);
    }
}



void Board::movePieceToDownward(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn){
    //delete current piece
    deletePiece(pieceMatrix,startingRow,startingColumn);
    bool canItMove=CanPieceMove(pieceMatrix,startingRow,startingColumn,startingRow+1,startingColumn);
    movePiece(pieceMatrix,startingRow,startingColumn,canItMove,startingRow+1,startingColumn);
}



void Board::rotateClockwise(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn, Piece piece, int& allowedRotationNumber, int& rotationCounter, bool& playerCanRotate){
    if(rotationCounter<allowedRotationNumber){
        rotationCounter++;
        deletePiece(pieceMatrix,startingRow,startingColumn);
        piece.rotateShapeRight(pieceMatrix);
        movePiece(pieceMatrix,startingRow,startingColumn,true,startingRow,startingColumn);
    }else{
        playerCanRotate=false;
    }
    
}



void Board::rotateCounterclockwise(int (&pieceMatrix)[3][3], int &startingRow, int &startingColumn, Piece piece, int& allowedRotationNumber, int& rotationCounter, bool& playerCanRotate){
    if(rotationCounter<allowedRotationNumber){
        rotationCounter++;
        deletePiece(pieceMatrix,startingRow,startingColumn);
        piece.rotateShapeLeft(pieceMatrix);
        movePiece(pieceMatrix,startingRow,startingColumn,true,startingRow,startingColumn);
    }else{
        playerCanRotate=false;
    }
}



void Board::drawNextPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&nextPiece)[3][3], sf::Font& font){
    //draw piece preview area
    sf::RectangleShape piecePreviewArea(sf::Vector2f(cellSize*5, cellSize*5));
    piecePreviewArea.setOutlineColor(getOutlineColor());                                                         // Set outline color
    piecePreviewArea.setOutlineThickness(2);                                                                    // Set outline thickness
    piecePreviewArea.setPosition(sf::Vector2f( cellSize*(getColumnSize()+2) , cellSize*(20-5)+20 ));
    piecePreviewArea.setFillColor(sf::Color::Transparent);
    window.draw(piecePreviewArea);
    //draw next piece into piece preview area
    for(int row=0;row<3;row++){
        for(int column=0;column<3;column++){
            if(nextPiece[row][column]==1){
                cell.setFillColor(getPieceColor());
                cell.setPosition(sf::Vector2f( cellSize*(getColumnSize()+2) + cellSize*column + cellSize  ,  cellSize*(20-5) + 20 + cellSize*row + cellSize ));
                window.draw(cell);
            }
        }
    }
    //write piece preview beneath area
    sf::Text piecePreviewText("Piece Preview",font,30);
    piecePreviewText.setFillColor(getOutlineColor());
    piecePreviewText.setPosition({static_cast<float>(cellSize*(getColumnSize()+2) ) , static_cast<float>(cellSize*(20-1) + 10 + cellSize)});
    window.draw(piecePreviewText);
}


void Board::holdPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&currentPiece)[3][3], sf::Font& font, bool& isHoldedAreaEmpty, int &startingRow, int &startingColumn, Piece piece, int (&holdedPiece)[3][3],int& currentPieceIndex, int& tempPieceIndex){
    //if holded piece area is empty at first
    if(isHoldedAreaEmpty==true){
        isHoldedAreaEmpty=false;
        //delete piece from gameBoard
        deletePiece(currentPiece,startingRow,startingColumn);
        //update holded piece matrix
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                holdedPiece[i][j]=currentPiece[i][j];
            }
        }
        //chose random piece
        int (&newPiece3)[3][3]=piece.chooseAPiece(currentPieceIndex,tempPieceIndex);
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                currentPiece[i][j]=newPiece3[i][j];
            }
        }
        //update gameBoard aaccordingly
        movePiece(currentPiece,startingRow,startingColumn, true,startingRow, startingColumn);
    }else{
        //delete piece from gameBoard
        deletePiece(currentPiece,startingRow,startingColumn);
        //create temporary matrix
        int tempMatrix[3][3];
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                tempMatrix[i][j]=holdedPiece[i][j];
            }
        }
        //update holded piece matrix
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                holdedPiece[i][j]=currentPiece[i][j];
            }
        }
        //update current piece
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                currentPiece[i][j]=tempMatrix[i][j];
            }
        }
        //update gameBoard aaccordingly
        movePiece(currentPiece,startingRow,startingColumn, true,startingRow, startingColumn);
    }
}



void Board::drawHoldPiece( sf::RenderWindow& window,sf::RectangleShape& cell, int cellSize, int (&holdedPiece)[3][3], sf::Font& font){
    //draw piece preview area
    sf::RectangleShape holdPieceArea(sf::Vector2f(cellSize*5, cellSize*5));
    holdPieceArea.setOutlineColor(getOutlineColor());                                                         // Set outline color
    holdPieceArea.setOutlineThickness(2);                                                                    // Set outline thickness
    holdPieceArea.setPosition(sf::Vector2f( cellSize*(getColumnSize()+2) , cellSize*(20-11)+20 ));
    holdPieceArea.setFillColor(sf::Color::Transparent);
    window.draw(holdPieceArea);
    //draw holded piece into area
    for(int row=0;row<3;row++){
        for(int column=0;column<3;column++){
            if(holdedPiece[row][column]==1){
                cell.setFillColor(getPieceColor());
                cell.setPosition(sf::Vector2f( cellSize*(getColumnSize()+2) + cellSize*column + cellSize  ,  cellSize*(20-11) + 20 + cellSize*row + cellSize ));
                window.draw(cell);
            }
        }
    }
    //write holde piece beneath holde piece area
    sf::Text holdPreviewText("Hold Preview",font,30);
    holdPreviewText.setFillColor(getOutlineColor());
    holdPreviewText.setPosition({static_cast<float>(cellSize*(getColumnSize()+2)), static_cast<float>(cellSize*(20-6)+10) });
    window.draw(holdPreviewText);
}

void Board::drawScore(int& score, sf::RenderWindow& window, int cellSize, sf::Font& font){
    //write Score Text
    sf::Text textScore("Score",font,40);
    textScore.setFillColor(getOutlineColor());
    textScore.setPosition({static_cast<float>(cellSize*(getColumnSize()+2)), static_cast<float>(cellSize) });
    window.draw(textScore);
    //write Score Value
    sf::Text textScoreValue(std::to_string(score),font,40);
    textScoreValue.setFillColor(getOutlineColor());
    textScoreValue.setPosition({static_cast<float>(cellSize*(getColumnSize()+2)), static_cast<float>(cellSize*2) });
    window.draw(textScoreValue);
}

void Board::increaseScore(int& score){
    score+=10;
}






bool Board::controlRowOfGameBoardMatrix( int& score, int scoreRise, int& lineClearedCounter){
    //create a boolean variable to hold value of wheter it is full of 1
    bool isThereAtLeastOneRowFullOfOne=false;
    bool isRowFullOfOne;
    int scoreMultiplier=0;
    for(int rowIndex=0;rowIndex<getRowSize();rowIndex++){
        isRowFullOfOne=true;
        for(int columnIndex=0;columnIndex<getColumnSize();columnIndex++){
            if(gameBoardMatrix(rowIndex,columnIndex)==0){
                isRowFullOfOne=false;
                break;
            }
        }
        //if row is full of one, make it all zero and carry upper shapes down
        if(isRowFullOfOne==true){
            lineClearedCounter++;
            isThereAtLeastOneRowFullOfOne=true;
            //make the row full of zero
            for(int i=0;i<getColumnSize();i++){
                gameBoardMatrix(rowIndex,i)=0;
            }
            //carry upper shapes down
            for(int i=rowIndex-1;i>0;i--){
                for(int j=0;j<getColumnSize();j++){
                    gameBoardMatrix(i+1,j)=gameBoardMatrix(i,j);
                }
            }
            scoreMultiplier+=1;
        }
    }
    //increase score value
    score+=scoreRise*scoreMultiplier;
    if(scoreMultiplier>1){
        score+=5*scoreMultiplier;
    }
    return isThereAtLeastOneRowFullOfOne;
}




//menu functions
void Board::pauseGame(bool& isPaused, bool& isGameStarted){
    isPaused=!isPaused;
    isGameStarted=true;
}
void Board::menuResume(bool& isPaused){
    isPaused=!isPaused;
}
void Board::menuNewGame(bool& isPaused, bool& isGameStarted,  int (&holdedPiece)[3][3], int& score, int& startinRow, int& startingColumn, bool& isNewGameSelected, bool& isHoldedAreaEmpty){
    isHoldedAreaEmpty=true;
    isPaused=!isPaused;
    isNewGameSelected=false;
    isGameStarted=true;
    //reset gameBoard
    for(int i=0;i<getRowSize();i++){
        for(int j=0; j<getColumnSize(); j++){
            if(gameBoardMatrix(i,j)==1)
                gameBoardMatrix(i,j)=0;
        }
    }
    //reset holded piece
    for(int i=0;i<3;i++){
        for(int j=0; j<3; j++){
            if(holdedPiece[i][j]==1)
                holdedPiece[i][j]=0;
        }
    }
    //set score zero
    score=0;
    //reset starting column and row
    startinRow=0;
    startingColumn=(gameBoardMatrix.getColumnSize()/2)-1;
}
void Board::menuThemeSelection(bool& isThemeSelectionSelected){
    isThemeSelectionSelected=true;
}





//theme functions
void Board::darkTheme(){
    backgroundColor=sf::Color::Black;
    outlineColor=sf::Color::White;
}
void Board::lightTheme(){
    backgroundColor=sf::Color::White;
    outlineColor=sf::Color::Black;
}
void Board::themeGoBackArrow(bool& isThemeSelectionSelected){
    isThemeSelectionSelected=false;
}







//newGame functions
void Board::newGameGoBack(bool& isNewGameSelected){
    isNewGameSelected=false;
}
//adjust custom piece
void Board::adjustCustomPiece(int (&customPiece)[3][3], int rowIndex, int columnIndex, sf::RectangleShape& customPieceCell, sf::RenderWindow& window, int positionX, int positionY, Piece& piece, int customPieceIndex){
    if(customPiece[rowIndex][columnIndex]==0){
        customPiece[rowIndex][columnIndex]=1;
        piece.pieceList[customPieceIndex][rowIndex][columnIndex]=1;
        customPieceCell.setFillColor(sf::Color::Red);
    }else{
        customPiece[rowIndex][columnIndex]=0;
        piece.pieceList[customPieceIndex][rowIndex][columnIndex]=0;
        customPieceCell.setFillColor(sf::Color::White);
    }
    customPieceCell.setPosition({static_cast<float>(positionX),static_cast<float>(positionY)});
    window.draw(customPieceCell);
}
//difficulty functions
void Board::easy(Piece& piece, sf::Window& window, float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit){
    piece.allowedRotationNumber=12;
    scoreRise=10;
    tetraminoDownwardSpeeed=1.0;
    blackoutScoreLimit=150;
    blackoutDurationNumber = 3.0f;
    portalScoreLimit=150;
    setRowSize(20);
}
void Board::medium(Piece& piece, sf::Window& window, float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit){
    piece.allowedRotationNumber=8;
    scoreRise=20;
    tetraminoDownwardSpeeed=0.65;
    blackoutScoreLimit=100;
    blackoutDurationNumber = 2.5f;
    portalScoreLimit=100;
    setRowSize(18);
}
void Board::hard(Piece& piece, sf::Window& window, float& tetraminoDownwardSpeeed,int& scoreRise,int& blackoutScoreLimit, double& blackoutDurationNumber, int& portalScoreLimit){
    piece.allowedRotationNumber=4;
    scoreRise=30;
    tetraminoDownwardSpeeed=0.25;
    blackoutScoreLimit=100;
    blackoutDurationNumber = 1.5f;
    portalScoreLimit=100;
    setRowSize(16);
}



std::pair<int, int> Board::findAPortalPlace(){
    //find a place to accomodate portal cells
    //portal is 2X2 matrix
    int startRow,startColumn;
    bool portalCellPlaceFound=true;
    for(int portalCounter=0;portalCounter<20;portalCounter++){
        startRow=createRandomNumbers(3,getRowSize()-3);
        startColumn=createRandomNumbers(0,getColumnSize()-3);
        //control if cells are empty
        for(int i=startRow;i<=(startRow+2);i++){
            for(int j=startColumn;j<=(startColumn+2);j++){
                if(gameBoardMatrix(i,j)==1){
                    portalCellPlaceFound=false;
                    break;
                }
            }
        }
        if(portalCellPlaceFound){
            break;
        }
    }
    //mark portal cells on gameBoardMatrix
    if(portalCellPlaceFound){
        for(int i=startRow;i<(startRow+2);i++){
            for(int j=startColumn;j<(startColumn+2);j++){
                gameBoardMatrix(i,j)=2;
            }
        }
    }
    return std::make_pair(startRow,startColumn);
}






//user ınput handling
//when game over write user name and its score to the file
//************************************************************************************************************************
//--------------------csvFilePath
std::string scoreBoardDataCSVFile="../../scoreBoardData/userScoreBoardData.csv";
//--------------------csvFilePath
//************************************************************************************************************************
void Board::writeUserToFile(std::string userName, int score){
    if(!findAndUpdateUser(userName,score)){
        std::ofstream csvFile(scoreBoardDataCSVFile, std::ios::app);
        if (!csvFile.is_open()) {
            std::cerr << "Error opening the file!" << std::endl;
        }
        //write username and score
        csvFile << userName << ";" << score <<"\n";
        //close file
        csvFile.close();
    }
    sortScoreBoard();
}
//find user if exist in database
bool Board::findAndUpdateUser(std::string playerName, int score){
    bool isUserFound=false;
    //create a user struct
    struct userData{
        std::string userName;
        int score;
    };
    //open the file
    std::ifstream inputFile(scoreBoardDataCSVFile);

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the CSV file!" << std::endl;
    }

    // Read data from the CSV file into a vector
    Vector<userData> userDataVector;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        userData userData;

        // Extract data from the line
        if (std::getline(iss, userData.userName, ';') && (iss >> userData.score)) {
            if(userData.userName==playerName){
                if(userData.score<score){
                    userData.score =score;
                }
                isUserFound=true;
            }
            userDataVector.pushBack(userData);
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    // Close the input file
    inputFile.close();
    // Open the CSV file for writing (overwrite mode)
    std::ofstream outputFile(scoreBoardDataCSVFile);
    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the CSV file for writing!" << std::endl;
    }
    // Write the modified data back to the CSV file
    for(auto& data:userDataVector){
        outputFile << data.userName << ";" << data.score << "\n";
    }
    // Close the output file
    outputFile.close();

    return isUserFound;
}

void Board::sortScoreBoard(){
    struct userData{
        std::string userName;
        int score;
    };

    //open the file
    std::ifstream inputFile(scoreBoardDataCSVFile);

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the CSV file!" << std::endl;
    }

    Stack<userData> userDataStack;           //top of stack holds max score
    Stack<userData> tempUserDataStack;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        userData userData;

        // Extract data from the line
        if (std::getline(iss, userData.userName, ';') && (iss >> userData.score)) {
            if(!userDataStack.isEmpty()){
                if( userData.score >= userDataStack.getTop().score ){
                    userDataStack.push(userData);
                }else{
                    while(!userDataStack.isEmpty()){
                        tempUserDataStack.push(userDataStack.getTop());
                        userDataStack.pop();
                        if(userData.score >= userDataStack.getTop().score){
                            userDataStack.push(userData);
                            while(!tempUserDataStack.isEmpty()){
                                userDataStack.push(tempUserDataStack.getTop());
                                tempUserDataStack.pop();
                            }
                            break;
                        }

                    }
                }
            }else{
                userDataStack.push(userData);
            }
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }
    // Close the input file
    inputFile.close();
    // Open the CSV file for writing (overwrite mode)
    std::ofstream outputFile(scoreBoardDataCSVFile);
    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the CSV file for writing!" << std::endl;
    }
    // Write the modified data back to the CSV file
    while(!userDataStack.isEmpty()){
        userData playerData=userDataStack.getTop();
        outputFile << playerData.userName << ";" << playerData.score << "\n";
        userDataStack.pop();
    }
    // Close the output file
    outputFile.close();
}


void Board::printScoreBoard(sf::Font& font,  int cellSize, sf::RenderWindow& window){
    std::ifstream inputFile(scoreBoardDataCSVFile);

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the CSV file!" << std::endl;
    }
    std::string line;
    std::string userName;
    std::string userData;
    int score;
    int padding=6;
    int index=1;
    while (std::getline(inputFile, line)) {
        if(index==9){
            break;
        }
        std::istringstream iss(line);

        // Extract data from the line
        int userNameLength;
        std::string spaceBetweenNameAndScore;
        if (std::getline(iss, userName, ';') && (iss >> score)) {
            userNameLength=userName.length();
            spaceBetweenNameAndScore.append(30-userNameLength,' ');
            userData= std::to_string(index) + ".  " + userName + spaceBetweenNameAndScore + std::to_string(score);
            sf::Text textUserData(userData,font,40);
            textUserData.setFillColor(getOutlineColor());
            textUserData.setPosition({ static_cast<float>(cellSize*2), static_cast<float>(cellSize*padding) });
            window.draw(textUserData);
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
        padding+=2;
        index++;
    }

    // Close the input file
    inputFile.close();
}


//************************************************************************************
//shrinking board functions
bool Board::controlIfEnoughRowAreEmpty(){
    bool isEnoughRowsAreEmpty=true;
    for(int i=0;i<=10;i++){
        for(int j=0;j<=(getColumnSize()-1);j++){
            if(gameBoardMatrix(i,j)==1){
                isEnoughRowsAreEmpty=false;
                break;
            }
        }
        if(!isEnoughRowsAreEmpty)
            break;
    }
    return isEnoughRowsAreEmpty;
}

void Board::movePiecesToUpShrinkingBoard(){
    for(int i=3;i<=(getRowSize()-1);i++){
        for(int j=0;j<=(getColumnSize()-1);j++){
            gameBoardMatrix(i-3,j)=gameBoardMatrix(i,j);
        }
    }
}

void Board::movePiecesToDownShrinkingBoard(){
    for(int i=(getRowSize()-4);i>=0;i--){
        for(int j=0;j<=(getColumnSize()-1);j++){
            gameBoardMatrix(i+3,j)=gameBoardMatrix(i,j);
        }
    }
}




void Board::restartBackgroundMusic(sf::Music& backgroundMusic){
    backgroundMusic.play();
}

