#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "../utils/Vector.h"
#include "../utils/utils.h"
#include "Piece.h"
#include <filesystem>
#include <atomic>

bool isPaused=true;
bool isGameStarted=false;
bool isThemeSelectionSelected=false;
bool isNewGameSelected=false;
bool isScoreBoardSelected=false;
bool isGameOver=false;
bool isUserNameTextAreaClicked=false;

//create a bool variable for rotation
bool playerCanRotate=true;

//for user name text create a int shift variable
int shiftName=0;
//score variable
int score=0;
int scoreRise=10;
//portal cell startRow and startColumn
int portalStartRow=0,portalStartColumn=0;
bool isPlayerInteractWithPortal=false;
bool isPortalCalled=false;
int newPieceplaceRow,newPiecePlaceColumn;

//soundPaths
const std::string selectionSoundPath="../../sounds/selectionSound.wav";
const std::string gameOverSoundPath="../../sounds/gameOverSound.wav";
const std::string dropSoundPath="../../sounds/dropSound.wav"; 
const std::string movePieceSoundPath="../../sounds/movePieceSound.wav";
const std::string clearRowSoundPath="../../sounds/clearRowSound.wav";
const std::string rotationSoundPath="../../sounds/rotationSound.wav";
const std::string tetrisBackgroundMusicPath="../../sounds/tetrisBackgroundMusic.mp3";



//set timer to control how tetramino goes down
sf::Clock clockForTetramino;
sf::Time elapsed;
float tetraminoDownwardSpeeed=0.5;


//create int piece and tempPiece index variable not to choose same piece sequencially
int currentPieceIndex=100;
int tempPieceIndex=100;


//ENVİRONMENTAL EFFECTS
//****** Screen blackout *******
//set a limit for screen blackout . when score reach that limit the screen will be got dark sometimes
int blackoutScoreLimit;
bool blackoutTimerStarted=false;
sf::Clock blackoutTimer;  // Create a clock to measure time
sf::Time blackoutElapsed;
double blackoutDurationNumber;
sf::Time blackoutDuration = sf::seconds(blackoutDurationNumber);

//portal score limit
int portalScoreLimit;




//selection indicator for easy, medium and hard choices
bool isEasySelected=false;
bool isMediumSelected=false;
bool isHardSelected=false;





void Game::startGame(){
    //create a gameBoard
    Board gameBoard;
    //create Piece Class
    Piece piece;
    // create the window
    size_t windowWidth=700;
    size_t windowHight=700;
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(windowWidth), static_cast<unsigned int>(windowHight)}), "tetris");
    // Set the frame rate limit to 10 frames per second
    window.setFramerateLimit(10);
    //set startingRow and startingColumn to move pieces
    int startingRow=0, startingColumn=(gameBoard.gameBoardMatrix.getColumnSize()/2)-1;
    //choose random Pieces
    int (&currentPiece)[3][3]=piece.chooseAPiece(currentPieceIndex, tempPieceIndex);
    int (&nextPiece)[3][3]=piece.chooseAPiece(currentPieceIndex, tempPieceIndex);
    //create a boolean variable for holded piece and create empty holded piece for start
    bool isHoldedAreaEmpty=true;
    int holdedPiece[3][3]={{0,0,0},{0,0,0},{0,0,0}};
    //**************************************************************************************************************
    //create a font 
    sf::Font font;
    std::filesystem::path filePath("../../fonts/Pixellettersfull-BnJ5.ttf");
    if (!font.loadFromFile(filePath))
    {
        std::cerr << "Error: Failed to load font from file '" << filePath << "'" << std::endl;
    }
    //************************************************************************************************************
    // determine cell size, padding and color
    float cellSize=gameBoard.getCellSize();
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setOutlineColor(gameBoard.getOutlineColor());     // Set outline color
    cell.setOutlineThickness(1);
    int padding=20;
    //custom pieces
    sf::RectangleShape customPieceCell({cellSize,cellSize});
    customPieceCell.setOutlineColor(gameBoard.getOutlineColor());
    customPieceCell.setOutlineThickness(2);
    //user namee text properties
    sf::Text userNameText("",font,40);
    userNameText.setFillColor(gameBoard.getOutlineColor());
    std::string userNameInput;
    //************************************************************************
    //create background music
    sf::Music backgroundMusic;
    if(!backgroundMusic.openFromFile(tetrisBackgroundMusicPath)) {
        // Handle error: Unable to load music
    }
    backgroundMusic.setLoop(false);
    bool shutBackgroundMusic=false;
    //************************************************************************
    //************************************************************************
    //upload Sound onand off images
    sf::Texture soundOn;
    if (!soundOn.loadFromFile("../../images/speakerOn.jpg")) {
        // Handle error if the image file can't be loaded
        std::cout<<"couldnt load speaker on image"<<std::endl;
    }
    sf::Texture soundOff;
    if (!soundOff.loadFromFile("../../images/speakerOff.jpg")) {
        // Handle error if the image file can't be loaded
        std::cout<<"couldnt load speaker off image"<<std::endl;
    }
    sf::Sprite spriteSoundOn(soundOn);
    sf::Sprite spriteSoundOff(soundOff);
    spriteSoundOn.setPosition(windowWidth-80,20);
    spriteSoundOff.setPosition(windowWidth-80,20);
    //************************************************************************

    //************************************************************************
    //create sound buffers
    sf::SoundBuffer bufferSelectionSound;
    if (!bufferSelectionSound.loadFromFile(selectionSoundPath)) {
        // Error handling: Unable to load the sound file
    }
    sf::SoundBuffer bufferGameOverSound;
    if (!bufferGameOverSound.loadFromFile(gameOverSoundPath)) {
        // Error handling: Unable to load the sound file
    }
    sf::SoundBuffer bufferDropSound;
    if (!bufferDropSound.loadFromFile(dropSoundPath)) {
        // Error handling: Unable to load the sound file
    }
    sf::SoundBuffer bufferMovePieceSound;
    if (!bufferMovePieceSound.loadFromFile(movePieceSoundPath)) {
        // Error handling: Unable to load the sound file
    }
    sf::SoundBuffer bufferClearRowSound;
    if (!bufferClearRowSound.loadFromFile(clearRowSoundPath)) {
        // Error handling: Unable to load the sound file
    }
    sf::SoundBuffer bufferRotationSound;
    if (!bufferRotationSound.loadFromFile(rotationSoundPath)) {
        // Error handling: Unable to load the sound file
    }

    sf::Sound selectionSound;
    selectionSound.setBuffer(bufferSelectionSound);
    sf::Sound gameOverSound;
    gameOverSound.setBuffer(bufferGameOverSound);
    sf::Sound dropSound;
    dropSound.setBuffer(bufferDropSound);
    sf::Sound movePieceSound;
    movePieceSound.setBuffer(bufferMovePieceSound);
    sf::Sound clearRowSound;
    clearRowSound.setBuffer(bufferClearRowSound);
    sf::Sound rotationSound;
    rotationSound.setBuffer(bufferRotationSound);
    //************************************************************************



    // run the program as long as the window is open
    while (window.isOpen())
    {
        //set speed od tetramino
        const sf::Time targetTime = sf::seconds(tetraminoDownwardSpeeed); // 1 second
        //set blackout duration
        blackoutDuration = sf::seconds(blackoutDurationNumber);

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) 
            {
                // Check which key is pressed
                // left key is pressed: move piece
                if (event.key.code == sf::Keyboard::Left){
                    // std::thread musicThread(&Board::playSound, &gameBoard, movePieceSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        movePieceSound.play();
                    gameBoard.movePieceToLeft(currentPiece, startingRow,startingColumn);
                }
                // right key is pressed: move piece    
                if (event.key.code == sf::Keyboard::Right){
                    // std::thread musicThread(&Board::playSound, &gameBoard, movePieceSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        movePieceSound.play();
                    gameBoard.movePieceToRight(currentPiece, startingRow,startingColumn);
                }
                // A key is pressed: rotate counterclockwise    
                if (event.key.code == sf::Keyboard::A && !isUserNameTextAreaClicked){
                    // std::thread musicThread(&Board::playSound, &gameBoard, rotationSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        rotationSound.play();
                    gameBoard.rotateCounterclockwise(currentPiece, startingRow,startingColumn, piece, piece.allowedRotationNumber, piece.rotationCounter, playerCanRotate);   

                }
                // D key is pressed: rotate clockwise 
                if (event.key.code == sf::Keyboard::D && !isUserNameTextAreaClicked){
                    // std::thread musicThread(&Board::playSound, &gameBoard, rotationSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        rotationSound.play();
                    gameBoard.rotateClockwise(currentPiece, startingRow,startingColumn, piece, piece.allowedRotationNumber, piece.rotationCounter, playerCanRotate);
                }
                if (event.key.code == sf::Keyboard::Down){
                    // std::thread musicThread(&Board::playSound, &gameBoard, movePieceSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        movePieceSound.play();
                    gameBoard.movePieceToDownward(currentPiece, startingRow,startingColumn);  
                }
                if (event.key.code == sf::Keyboard::Space && !isUserNameTextAreaClicked){
                    // std::thread musicThread(&Board::playSound, &gameBoard, dropSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        dropSound.play();
                    for(int i=0;i<20;i++)
                        gameBoard.movePieceToDownward(currentPiece, startingRow,startingColumn);      
                }
                if (event.key.code == sf::Keyboard::S && !isUserNameTextAreaClicked){
                    // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        selectionSound.play();
                    gameBoard.holdPiece(window, cell, cellSize, currentPiece, font, isHoldedAreaEmpty, startingRow, startingColumn, piece, holdedPiece,currentPieceIndex, tempPieceIndex);
                }
                if (event.key.code == sf::Keyboard::P && !isUserNameTextAreaClicked){
                    // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        selectionSound.play();
                    //pause background music
                    backgroundMusic.pause();
                    gameBoard.pauseGame(isPaused,isGameStarted);
                }
            }else if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    // Check where mouse is clicked for pause menu
                    if(event.mouseButton.x>=(windowWidth/2)-60 && event.mouseButton.x<=((windowWidth/2)+60) && event.mouseButton.y>=cellSize*5 && event.mouseButton.y<=cellSize*7 && isPaused && !isThemeSelectionSelected && !isNewGameSelected && !isGameOver && !isScoreBoardSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        //play background music
                        if(!shutBackgroundMusic)
                            backgroundMusic.play();
                        gameBoard.menuResume(isPaused);

                    }else if(event.mouseButton.x>=(windowWidth/2)-70 && event.mouseButton.x<=((windowWidth/2)+70) && event.mouseButton.y>=cellSize*7 && event.mouseButton.y<=cellSize*9 && isPaused && !isThemeSelectionSelected && !isNewGameSelected && !isGameOver && !isScoreBoardSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isNewGameSelected=true;
                    }else if(event.mouseButton.x>=(windowWidth/2)-90 && event.mouseButton.x<=((windowWidth/2)+90) && event.mouseButton.y>=cellSize*9 && event.mouseButton.y<=cellSize*11 && isPaused && !isThemeSelectionSelected && !isNewGameSelected && !isGameOver && !isScoreBoardSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isScoreBoardSelected=true;
                    }else if(event.mouseButton.x>=(windowWidth/2)-90 && event.mouseButton.x<=((windowWidth/2)+90) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*13 && isPaused && !isThemeSelectionSelected && !isNewGameSelected && !isGameOver && !isScoreBoardSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isThemeSelectionSelected=true;
                    }else if(event.mouseButton.x>=(windowWidth/2)-50 && event.mouseButton.x<=((windowWidth/2)+50) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*15 && isPaused && !isThemeSelectionSelected && !isNewGameSelected && !isGameOver && !isScoreBoardSelected){
                        //QUİT FROM GAME
                        window.close();
                    }
                    // Check where mouse is clicked for theme selection menu 
                    if(event.mouseButton.x>=((windowWidth/2)-50) && event.mouseButton.x<=((windowWidth/2)+50) && event.mouseButton.y>=cellSize*4 && event.mouseButton.y<=cellSize*6 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.darkTheme();
                    }else if(event.mouseButton.x>=((windowWidth/2)-50) && event.mouseButton.x<=((windowWidth/2)+50) && event.mouseButton.y>=cellSize*6 && event.mouseButton.y<=cellSize*8 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.lightTheme();
                    }else if(event.mouseButton.x>=(cellSize) && event.mouseButton.x<=(cellSize*4) && event.mouseButton.y>=cellSize*2 && event.mouseButton.y<=cellSize*4 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.themeGoBackArrow(isThemeSelectionSelected);
                    }else if(event.mouseButton.x>=(cellSize*4) && event.mouseButton.x<=(cellSize*5) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[0]);
                    }else if(event.mouseButton.x>=(cellSize*6) && event.mouseButton.x<=(cellSize*7) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[1]);
                    }else if(event.mouseButton.x>=(cellSize*8) && event.mouseButton.x<=(cellSize*9) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[2]);
                    }else if(event.mouseButton.x>=(cellSize*10) && event.mouseButton.x<=(cellSize*11) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[3]);
                    }else if(event.mouseButton.x>=(cellSize*12) && event.mouseButton.x<=(cellSize*13) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[4]);
                    }else if(event.mouseButton.x>=(cellSize*14) && event.mouseButton.x<=(cellSize*15) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[5]);
                    }else if(event.mouseButton.x>=(cellSize*16) && event.mouseButton.x<=(cellSize*17) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[6]);
                    }else if(event.mouseButton.x>=(cellSize*18) && event.mouseButton.x<=(cellSize*19) && event.mouseButton.y>=cellSize*11 && event.mouseButton.y<=cellSize*12 && isPaused && isThemeSelectionSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.setPieceColor(gameBoard.colors[7]);
                    }
                    //check user ınput for his/her name, game difficulty sttings and custom piece adding
                    if(event.mouseButton.x>=((windowWidth/2)-150) && event.mouseButton.x<=((windowWidth/2)+150) && event.mouseButton.y>=cellSize*3 && event.mouseButton.y<=cellSize*4 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isUserNameTextAreaClicked=true;
                    }else if(event.mouseButton.x>=((windowWidth/2)+170) && event.mouseButton.x<=((windowWidth/2)+250) && event.mouseButton.y>=cellSize*3 && event.mouseButton.y<=cellSize*5 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isUserNameTextAreaClicked=false;
                    }else if(event.mouseButton.x>=(cellSize) && event.mouseButton.x<=(cellSize*4) && event.mouseButton.y>=cellSize*2 && event.mouseButton.y<=cellSize*4 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.newGameGoBack(isNewGameSelected);
                    }else if(event.mouseButton.x>=(cellSize*5) && event.mouseButton.x<=(cellSize*8) && event.mouseButton.y>=cellSize*7 && event.mouseButton.y<=cellSize*9 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.easy(piece,window,tetraminoDownwardSpeeed,scoreRise,blackoutScoreLimit,blackoutDurationNumber,portalScoreLimit);
                        isEasySelected=true;
                        isMediumSelected=false;
                        isHardSelected=false;
                    }else if(event.mouseButton.x>=(cellSize*10) && event.mouseButton.x<=(cellSize*13) && event.mouseButton.y>=cellSize*7 && event.mouseButton.y<=cellSize*9 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.medium(piece, window,tetraminoDownwardSpeeed,scoreRise,blackoutScoreLimit,blackoutDurationNumber,portalScoreLimit);
                        isEasySelected=false;
                        isMediumSelected=true;
                        isHardSelected=false;
                    }else if(event.mouseButton.x>=(cellSize*16) && event.mouseButton.x<=(cellSize*19) && event.mouseButton.y>=cellSize*7 && event.mouseButton.y<=cellSize*9 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.hard(piece,window,tetraminoDownwardSpeeed,scoreRise,blackoutScoreLimit,blackoutDurationNumber,portalScoreLimit);
                        isEasySelected=false;
                        isMediumSelected=false;
                        isHardSelected=true;
                    }else if(event.mouseButton.x>=((windowWidth/2)-80) && event.mouseButton.x<=((windowWidth/2)+80) && event.mouseButton.y>=cellSize*18 && event.mouseButton.y<=cellSize*20 && isPaused && isNewGameSelected){
                        
                        //start game and play background music
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        //play background music
                        if(!shutBackgroundMusic)
                            backgroundMusic.play();
                        gameBoard.menuNewGame(isPaused, isGameStarted, holdedPiece, score, startingRow,startingColumn,isNewGameSelected, isHoldedAreaEmpty);
                    
                    }else if(event.mouseButton.x>=(cellSize*5) && event.mouseButton.x<=(cellSize*6) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        //adjust custom piece one
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 0, 0,customPieceCell,window,cellSize*5,cellSize*13,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*6) && event.mouseButton.x<=(cellSize*7) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 0, 1,customPieceCell,window,cellSize*6,cellSize*13,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*7) && event.mouseButton.x<=(cellSize*8) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 0, 2,customPieceCell,window,cellSize*7,cellSize*13,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*5) && event.mouseButton.x<=(cellSize*6) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 1, 0,customPieceCell,window,cellSize*5,cellSize*14,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*6) && event.mouseButton.x<=(cellSize*7) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 1, 1,customPieceCell,window,cellSize*6,cellSize*14,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*7) && event.mouseButton.x<=(cellSize*8) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 1, 2,customPieceCell,window,cellSize*7,cellSize*14,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*5) && event.mouseButton.x<=(cellSize*6) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 2, 0,customPieceCell,window,cellSize*5,cellSize*15,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*6) && event.mouseButton.x<=(cellSize*7) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 2, 1,customPieceCell,window,cellSize*6,cellSize*15,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*7) && event.mouseButton.x<=(cellSize*8) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape1, 2, 2,customPieceCell,window,cellSize*7,cellSize*15,piece,10);
                    }else if(event.mouseButton.x>=(cellSize*10) && event.mouseButton.x<=(cellSize*11) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        //adjust custom piece two
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 0, 0,customPieceCell,window,cellSize*10,cellSize*13,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*11) && event.mouseButton.x<=(cellSize*12) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 0, 1,customPieceCell,window,cellSize*11,cellSize*13,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*12) && event.mouseButton.x<=(cellSize*13) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 0, 2,customPieceCell,window,cellSize*12,cellSize*13,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*10) && event.mouseButton.x<=(cellSize*11) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 1, 0,customPieceCell,window,cellSize*10,cellSize*14,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*11) && event.mouseButton.x<=(cellSize*12) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 1, 1,customPieceCell,window,cellSize*11,cellSize*14,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*12) && event.mouseButton.x<=(cellSize*13) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 1, 2,customPieceCell,window,cellSize*12,cellSize*14,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*10) && event.mouseButton.x<=(cellSize*11) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 2, 0,customPieceCell,window,cellSize*10,cellSize*15,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*11) && event.mouseButton.x<=(cellSize*12) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 2, 1,customPieceCell,window,cellSize*11,cellSize*15,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*12) && event.mouseButton.x<=(cellSize*13) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape2, 2, 2,customPieceCell,window,cellSize*12,cellSize*15,piece,11);
                    }else if(event.mouseButton.x>=(cellSize*15) && event.mouseButton.x<=(cellSize*16) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        //adjust custom piece three
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 0, 0,customPieceCell,window,cellSize*15,cellSize*13,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*16) && event.mouseButton.x<=(cellSize*17) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 0, 1,customPieceCell,window,cellSize*11,cellSize*13,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*17) && event.mouseButton.x<=(cellSize*18) && event.mouseButton.y>=cellSize*13 && event.mouseButton.y<=cellSize*14 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 0, 2,customPieceCell,window,cellSize*12,cellSize*13,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*15) && event.mouseButton.x<=(cellSize*16) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 1, 0,customPieceCell,window,cellSize*10,cellSize*14,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*16) && event.mouseButton.x<=(cellSize*17) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 1, 1,customPieceCell,window,cellSize*11,cellSize*14,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*17) && event.mouseButton.x<=(cellSize*18) && event.mouseButton.y>=cellSize*14 && event.mouseButton.y<=cellSize*15 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 1, 2,customPieceCell,window,cellSize*12,cellSize*14,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*15) && event.mouseButton.x<=(cellSize*16) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 2, 0,customPieceCell,window,cellSize*10,cellSize*15,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*16) && event.mouseButton.x<=(cellSize*17) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 2, 1,customPieceCell,window,cellSize*11,cellSize*15,piece,12);
                    }else if(event.mouseButton.x>=(cellSize*17) && event.mouseButton.x<=(cellSize*18) && event.mouseButton.y>=cellSize*15 && event.mouseButton.y<=cellSize*16 && isPaused && isNewGameSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        gameBoard.adjustCustomPiece(piece.exclusiveShape3, 2, 2,customPieceCell,window,cellSize*12,cellSize*15,piece,12);
                    }
                    // Check where mouse is clicked for game over menu 
                    if(event.mouseButton.x>=((windowWidth/2)-150) && event.mouseButton.x<=((windowWidth/2)+150) && event.mouseButton.y>=cellSize*10 && event.mouseButton.y<=cellSize*12 && isPaused && isGameOver){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isGameOver=false;
                    }
                    // Check where mouse is clicked for score board menu 
                    if(event.mouseButton.x>=(cellSize) && event.mouseButton.x<=(cellSize*3) && event.mouseButton.y>=cellSize*2 && event.mouseButton.y<=cellSize*4 && isPaused && isScoreBoardSelected){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        if(!shutBackgroundMusic)
                            selectionSound.play();
                        isScoreBoardSelected=false;
                    }
                    // Check where mouse is clicked for sound on and off
                    if(event.mouseButton.x>=(windowWidth-80) && event.mouseButton.x<=(windowWidth-20) && event.mouseButton.y>=20 && event.mouseButton.y<=80 ){
                        // std::thread musicThread(&Board::playSound, &gameBoard, selectionSoundPath);
                        // musicThread.detach();
                        shutBackgroundMusic=!shutBackgroundMusic;
                        if(shutBackgroundMusic)
                            backgroundMusic.pause();
                    }
                }
            }else if (event.type == sf::Event::TextEntered && isUserNameTextAreaClicked) {
                // Handle the ASCII characters (excluding control characters)
                if (event.text.unicode < 128 && event.text.unicode != 8) {
                    userNameInput += static_cast<char>(event.text.unicode);
                    userNameText.setString(userNameInput);
                    shiftName-=8;
                    userNameText.setPosition({static_cast<float>((windowWidth/2)+shiftName),static_cast<float>(cellSize*3)});
                } else if (event.text.unicode == 8 && !userNameInput.empty()) {
                    // Backspace to remove the last character
                    userNameInput.pop_back();
                    userNameText.setString(userNameInput);
                    shiftName+=8;
                    userNameText.setPosition({static_cast<float>((windowWidth/2)+shiftName),static_cast<float>(cellSize*3)});
                }
            }
        }



        // clear the window with background color
        window.clear(gameBoard.getBackgroundColor());



        //if game is paused, show paused screen
        if(isPaused){
            if(isThemeSelectionSelected)
            {
                //show themeSelection part if selected
                sf::Text textSelectTheme("Select a theme",font,60);
                textSelectTheme.setFillColor(gameBoard.getOutlineColor());
                textSelectTheme.setPosition({static_cast<float>(windowWidth/2)-150, static_cast<float>(cellSize*1) });
                window.draw(textSelectTheme);
                //dark theme
                sf::Text textDarkTheme("Dark",font,40);
                textDarkTheme.setFillColor(gameBoard.getOutlineColor());
                textDarkTheme.setPosition({static_cast<float>(windowWidth/2)-50, static_cast<float>(cellSize*4) });
                window.draw(textDarkTheme);
                //Light theme
                sf::Text textLightTheme("Light",font,40);
                textLightTheme.setFillColor(gameBoard.getOutlineColor());
                textLightTheme.setPosition({static_cast<float>(windowWidth/2)-50, static_cast<float>(cellSize*6) });
                window.draw(textLightTheme);
                //theme go back arrow
                sf::Text textThemeGoBack(" <== ",font,40);
                textThemeGoBack.setFillColor(gameBoard.getOutlineColor());
                textThemeGoBack.setPosition({static_cast<float>(cellSize), static_cast<float>(cellSize*2) });
                window.draw(textThemeGoBack);
                //select piece color
                sf::Text textSelectPieceColor("Select piece Color",font,60);
                textSelectPieceColor.setFillColor(gameBoard.getOutlineColor());
                textSelectPieceColor.setPosition({ static_cast<float>((windowWidth/2)-200), static_cast<float>(cellSize*8) });
                window.draw(textSelectPieceColor);
                //piece colors
                cell.setOutlineColor(gameBoard.getOutlineColor());
                cell.setFillColor(sf::Color::Red);
                cell.setPosition({cellSize*4, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Blue);
                cell.setPosition({cellSize*6, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Yellow);
                cell.setPosition({cellSize*8, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Green);
                cell.setPosition({cellSize*10, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Magenta);
                cell.setPosition({cellSize*12, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Cyan);
                cell.setPosition({cellSize*14, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::Black);
                cell.setPosition({cellSize*16, cellSize*11});
                window.draw(cell);
                cell.setFillColor(sf::Color::White);
                cell.setPosition({cellSize*18, cellSize*11});
                window.draw(cell);
            }else if(isNewGameSelected)
            { 
                //show New game part if selected
                sf::Text textEnterName("Enter Your Name",font,40);
                textEnterName.setFillColor(gameBoard.getOutlineColor());
                textEnterName.setPosition({static_cast<float>(windowWidth/2)-120, static_cast<float>(cellSize*1) });
                window.draw(textEnterName);
                //text area for user name
                sf::RectangleShape userInputArea({300,50});
                userInputArea.setOutlineColor(gameBoard.getOutlineColor());
                userInputArea.setOutlineThickness(2);
                userInputArea.setFillColor(sf::Color::Transparent);
                userInputArea.setPosition({static_cast<float>(windowWidth/2)-150 , static_cast<float>(cellSize*3) });
                window.draw(userInputArea);
                //draw user name
                userNameText.setFillColor(gameBoard.getOutlineColor());
                window.draw(userNameText);
                //finish getting text
                sf::Text textUserNameEnd("enter",font,40);
                textUserNameEnd.setFillColor(gameBoard.getOutlineColor());
                textUserNameEnd.setPosition({static_cast<float>(windowWidth/2)+170, static_cast<float>(cellSize*3) });
                window.draw(textUserNameEnd);
                //New game go back arrow
                sf::Text textNewGameGoBack(" <== ",font,40);
                textNewGameGoBack.setFillColor(gameBoard.getOutlineColor());
                textNewGameGoBack.setPosition({static_cast<float>(cellSize), static_cast<float>(cellSize*2) });
                window.draw(textNewGameGoBack);
                //difficulty settings
                sf::Text textDifficulty(" Select Difficulty ",font,40);
                textDifficulty.setFillColor(gameBoard.getOutlineColor());
                textDifficulty.setPosition({static_cast<float>((windowWidth/2)-120), static_cast<float>(cellSize*5) });
                window.draw(textDifficulty);
                //easy
                sf::Text textEasy("Easy",font,40);
                textEasy.setFillColor(gameBoard.getOutlineColor());
                textEasy.setPosition({static_cast<float>(cellSize*5), static_cast<float>(cellSize*7) });
                window.draw(textEasy);
                if(isEasySelected){
                    sf::RectangleShape textIndicatorLine({50,5});
                    textIndicatorLine.setFillColor(gameBoard.getOutlineColor());
                    textIndicatorLine.setPosition({static_cast<float>(cellSize*5), static_cast<float>(cellSize*9) });
                    window.draw(textIndicatorLine);
                }

                //medium
                sf::Text textMedium("Medium",font,40);
                textMedium.setFillColor(gameBoard.getOutlineColor());
                textMedium.setPosition({static_cast<float>(cellSize*10), static_cast<float>(cellSize*7) });
                window.draw(textMedium);
                if(isMediumSelected){
                    sf::RectangleShape textIndicatorLine({50,5});
                    textIndicatorLine.setFillColor(gameBoard.getOutlineColor());
                    textIndicatorLine.setPosition({static_cast<float>(cellSize*10), static_cast<float>(cellSize*9) });
                    window.draw(textIndicatorLine);
                }
                //hard
                sf::Text textHard("Hard",font,40);
                textHard.setFillColor(gameBoard.getOutlineColor());
                textHard.setPosition({static_cast<float>(cellSize*16), static_cast<float>(cellSize*7) });
                window.draw(textHard);
                if(isHardSelected){
                    sf::RectangleShape textIndicatorLine({50,5});
                    textIndicatorLine.setFillColor(gameBoard.getOutlineColor());
                    textIndicatorLine.setPosition({static_cast<float>(cellSize*16), static_cast<float>(cellSize*9) });
                    window.draw(textIndicatorLine);
                }
                //Enter custom piece
                sf::Text textCustomPiece("Draw your custom piece",font,40);
                textCustomPiece.setFillColor(gameBoard.getOutlineColor());
                textCustomPiece.setPosition({static_cast<float>((windowWidth/2)-150), static_cast<float>(cellSize*10) });
                window.draw(textCustomPiece);
                //custom piece 1
                customPieceCell.setOutlineColor(gameBoard.getOutlineColor());
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        if(piece.exclusiveShape1[i][j]==1)
                            customPieceCell.setFillColor(gameBoard.getPieceColor());
                        else
                            customPieceCell.setFillColor(sf::Color::Transparent);    
                        customPieceCell.setPosition({(cellSize*5+cellSize*j),(cellSize*13+cellSize*i)});
                        window.draw(customPieceCell);
                    }
                }
                //custom piece 2
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        if(piece.exclusiveShape2[i][j]==1)
                            customPieceCell.setFillColor(gameBoard.getPieceColor());
                        else
                            customPieceCell.setFillColor(sf::Color::Transparent);
                        customPieceCell.setPosition({(cellSize*10+cellSize*j),(cellSize*13+cellSize*i)});
                        window.draw(customPieceCell);
                    }
                }
                //custom piece 3
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        if(piece.exclusiveShape3[i][j]==1)
                            customPieceCell.setFillColor(gameBoard.getPieceColor());
                        else
                            customPieceCell.setFillColor(sf::Color::Transparent);
                        customPieceCell.setPosition({(cellSize*15+cellSize*j),(cellSize*13+cellSize*i)});
                        window.draw(customPieceCell);
                    }
                }
                //start game
                sf::Text textStartGame("Start game",font,40);
                textStartGame.setFillColor(gameBoard.getOutlineColor());
                textStartGame.setPosition({static_cast<float>((windowWidth/2)-80), static_cast<float>(cellSize*18) });
                window.draw(textStartGame);
            }else if(isGameOver){
                //write game over
                sf::Text textGameOver("GAME OVER",font,60);
                textGameOver.setFillColor(gameBoard.getOutlineColor());
                textGameOver.setPosition({static_cast<float>(windowWidth/2)-130, static_cast<float>(cellSize*3) });
                window.draw(textGameOver);
                //write user score
                sf::Text textGameOverScore("your score",font,40);
                textGameOverScore.setFillColor(gameBoard.getOutlineColor());
                textGameOverScore.setPosition({static_cast<float>(windowWidth/2)-80, static_cast<float>(cellSize*6) });
                window.draw(textGameOverScore);
                sf::Text textGameOverScoreValue(std::to_string(score),font,40);
                textGameOverScoreValue.setFillColor(gameBoard.getOutlineColor());
                textGameOverScoreValue.setPosition({static_cast<float>(windowWidth/2)-20, static_cast<float>(cellSize*8) });
                window.draw(textGameOverScoreValue);
                //back to menu
                sf::Text textGameOverBackToMenu("<-- back to menu <--",font,40);
                textGameOverBackToMenu.setFillColor(gameBoard.getOutlineColor());
                textGameOverBackToMenu.setPosition({static_cast<float>(windowWidth/2)-150, static_cast<float>(cellSize*10) });
                window.draw(textGameOverBackToMenu);
            }else if(isScoreBoardSelected){
                //Score Board
                sf::Text textScoreBoardHeader("Score Board",font,60);
                textScoreBoardHeader.setFillColor(gameBoard.getOutlineColor());
                textScoreBoardHeader.setPosition({static_cast<float>(windowWidth/2)-150, static_cast<float>(cellSize*2) });
                window.draw(textScoreBoardHeader);
                //Score boaard go back arrow
                sf::Text textScoreBoardGoBack(" <== ",font,40);
                textScoreBoardGoBack.setFillColor(gameBoard.getOutlineColor());
                textScoreBoardGoBack.setPosition({static_cast<float>(cellSize), static_cast<float>(cellSize*2) });
                window.draw(textScoreBoardGoBack);
                //print user score datas
                gameBoard.printScoreBoard(font, cellSize, window);
            }else 
            {
                //show pause screen 
                //write header
                sf::Text header("TETRIS",font,60);
                header.setFillColor(gameBoard.getOutlineColor());
                header.setPosition({static_cast<float>(windowWidth/2)-80, static_cast<float>(cellSize) });
                window.draw(header);
                //write resume if game has been started
                if(isGameStarted){
                    sf::Text textResume("Resume",font,40);
                    textResume.setFillColor(gameBoard.getOutlineColor());
                    textResume.setPosition({static_cast<float>(windowWidth/2)-60, static_cast<float>(cellSize*5) });
                    window.draw(textResume);
                } 
                //write start game option
                sf::Text textNewGame("New Game",font,40);
                textNewGame.setFillColor(gameBoard.getOutlineColor());
                textNewGame.setPosition({static_cast<float>(windowWidth/2)-70, static_cast<float>(cellSize*7) });
                window.draw(textNewGame);
                //write score board option
                sf::Text textScoreBoard("Score Board",font,40);
                textScoreBoard.setFillColor(gameBoard.getOutlineColor());
                textScoreBoard.setPosition({static_cast<float>(windowWidth/2)-90, static_cast<float>(cellSize*9) });
                window.draw(textScoreBoard);
                //write theme selection option
                sf::Text textSelectTheme("Select Theme",font,40);
                textSelectTheme.setFillColor(gameBoard.getOutlineColor());
                textSelectTheme.setPosition({static_cast<float>(windowWidth/2)-90, static_cast<float>(cellSize*11) });
                window.draw(textSelectTheme);
                //write quit option
                sf::Text textQuit("Quit",font,40);
                textQuit.setFillColor(gameBoard.getOutlineColor());
                textQuit.setPosition({static_cast<float>(windowWidth/2)-50, static_cast<float>(cellSize*13) });
                window.draw(textQuit);
                //write Developer 
                sf::Text textDeveloper("Developer",font,40);
                textDeveloper.setFillColor(gameBoard.getOutlineColor());
                textDeveloper.setPosition({static_cast<float>(windowWidth/2)-70, static_cast<float>(windowHight-150) });
                window.draw(textDeveloper);
                //write Developer Name
                sf::Text textDeveloperName("ABDULLAH TAPANCI",font,40);
                textDeveloperName.setFillColor(gameBoard.getOutlineColor());
                textDeveloperName.setPosition({static_cast<float>(windowWidth/2)-130, static_cast<float>(windowHight-100) });
                window.draw(textDeveloperName);
            }
        }else if(!isPaused)
        {
            //if is not paused start game logic

            //try to put piece into gameBoard
            bool isPut=gameBoard.putAPieceToTheGameBoard(currentPiece);

            //move piece
            bool isMoving=gameBoard.CanPieceMove(currentPiece,startingRow,startingColumn,startingRow+1,startingColumn);
            if(isMoving==0){
                if(isPut==0){
                    //!!!!!!!!!!GAME OVER!!!!!!!!!!!!
                    backgroundMusic.pause();
                    // std::thread musicThread(&Board::playSound, &gameBoard, gameOverSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        gameOverSound.play();
                    isPaused=true;
                    isGameStarted=false;
                    isGameOver=true;
                    gameBoard.writeUserToFile(userNameInput,score);
                }
                gameBoard.movePiece(currentPiece,startingRow,startingColumn,isMoving,startingRow+1,startingColumn);
                bool isRowFullOfOne=gameBoard.controlRowOfGameBoardMatrix(score,scoreRise);
                if(isRowFullOfOne){
                    // std::thread musicThread(&Board::playSound, &gameBoard, clearRowSoundPath);
                    // musicThread.detach();
                    if(!shutBackgroundMusic)
                        clearRowSound.play();
                }
            
                //reset variables
                startingRow=0; 
                startingColumn=(gameBoard.gameBoardMatrix.getColumnSize()/2)-1;
                piece.rotationCounter=0;
                playerCanRotate=true;
                isPlayerInteractWithPortal=false;
                //copy nextPiece to current piece
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        currentPiece[i][j]=nextPiece[i][j];
                    }
                }
                //choose a random nextPiece
                int (&newPiece2)[3][3]=piece.chooseAPiece(currentPieceIndex, tempPieceIndex);
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        nextPiece[i][j]=newPiece2[i][j];
                    }
                }
                //********* if score reaches the blackoutLimit set a timer if a spesific condition is met **********
                if(score>=blackoutScoreLimit){
                    if(createRandomNumbers(0,20)<5){
                        blackoutTimer.restart();
                        blackoutTimerStarted=true;
                    }
                }
                //************************************************************************************
                //delete portal that player didnt interact with it
                for(int i=0;i<=(gameBoard.getRowSize()-1);i++){
                    for(int j=0;j<=(gameBoard.getColumnSize()-1);j++){
                        if(gameBoard.gameBoardMatrix(i,j)==2)
                            gameBoard.gameBoardMatrix(i,j)=0;
                    }
                }
                isPlayerInteractWithPortal=false; 
                startingRow=0; 
                startingColumn=(gameBoard.gameBoardMatrix.getColumnSize()/2)-1;

                if( (score>=portalScoreLimit) && (createRandomNumbers(0,10)<3) ){
                    //find a portal and set int portalStartRow ,portalStartColumn
                    std::pair<int, int>  result=gameBoard.findAPortalPlace();
                    portalStartRow=result.first;
                    portalStartColumn=result.second;
                    isPortalCalled=true;
                }
                 //************************************************************************************


            }else{
                //control if portal is called 
                 //************************************************************************************
                if(isPortalCalled){
                    //control portal and piece whether they interact
                    for(int i=portalStartRow;i<=(portalStartRow+1);i++){
                        for(int j=portalStartColumn;j<=(portalStartColumn+1);j++){
                            if(gameBoard.gameBoardMatrix(i,j)==0){
                                isPlayerInteractWithPortal=true;
                                break;
                            }
                        }
                        if(isPlayerInteractWithPortal){
                            break;
                        }
                    }
                    //if player interact with portal find a position and move piece there
                    if(isPlayerInteractWithPortal)
                    {
                        //find a place
                        bool isPlaceFound;
                        while(true)
                        {
                            isPlaceFound=true;
                            newPieceplaceRow=createRandomNumbers(0,gameBoard.getRowSize()-3);
                            newPiecePlaceColumn=createRandomNumbers(0,gameBoard.getColumnSize()-3);
                            int currPieceRowIndex=0;
                            int currPieceColumnIndex=0;
                            for(int i=newPieceplaceRow;i<=(newPieceplaceRow+2);i++){
                                currPieceColumnIndex=0;
                                for(int j=newPiecePlaceColumn;j<=(newPiecePlaceColumn+2);j++){
                                    if(gameBoard.gameBoardMatrix(i,j)==1 && currentPiece[currPieceRowIndex][currPieceColumnIndex]==1){
                                        isPlaceFound=false;
                                        break;
                                    }
                                    currPieceColumnIndex++;
                                }
                                if(!isPlaceFound)
                                    break;
                                currPieceRowIndex++;
                            }
                            if(!isPlaceFound)
                                continue;
                            //if place is found delete piece and move it to new place
                            if(isPlaceFound){
                                gameBoard.deletePiece(currentPiece,startingRow,startingColumn);
                                //delete portal
                                for(int i=0;i<=(gameBoard.getRowSize()-1);i++){
                                    for(int j=0;j<=(gameBoard.getColumnSize()-1);j++){
                                        if(gameBoard.gameBoardMatrix(i,j)==2)
                                            gameBoard.gameBoardMatrix(i,j)=0;
                                    }
                                }
                                gameBoard.movePiece(currentPiece,startingRow,startingColumn,isMoving,newPiecePlaceColumn,newPiecePlaceColumn);
                                gameBoard.deletePiece(currentPiece,startingRow,startingColumn);
                                break;
                            }
                        }
                        isPortalCalled=false;
                        isPlayerInteractWithPortal=false;
                        portalStartRow=0;
                        portalStartColumn=0;
                    }

                }
                //************************************************************************************************************************

                //move automatically tetramino to downward
                elapsed = clockForTetramino.getElapsedTime();
                if (elapsed >= targetTime){
                    gameBoard.movePiece(currentPiece,startingRow,startingColumn,isMoving,startingRow+1,startingColumn);
                    clockForTetramino.restart();
                }else{
                    gameBoard.movePiece(currentPiece,startingRow,startingColumn,isMoving,startingRow,startingColumn);
                }
                //check whether background music is playing. ıf not restart it
                if(backgroundMusic.getStatus() != sf::Music::Playing){
                    if(!shutBackgroundMusic)
                    backgroundMusic.play();
                }
            }


            //check blackout conditions if they are met
            if(blackoutTimerStarted && (blackoutTimer.getElapsedTime()<blackoutDuration)){
                window.clear(sf::Color::Black);
            }else{
                //draw gameBoard
                cell.setOutlineColor(gameBoard.getOutlineColor());
                for(int row=0;row<gameBoard.getRowSize();row++){
                    for(int column=0;column<gameBoard.getColumnSize();column++){
                        cell.setPosition(sf::Vector2f(cellSize * column+padding, cellSize * row+padding));
                        if(gameBoard.gameBoardMatrix(row,column)==0)
                            cell.setFillColor(sf::Color::Transparent); // Set fill color to transparent
                        if(gameBoard.gameBoardMatrix(row,column)==1)
                            cell.setFillColor(gameBoard.getPieceColor()); // Set fill color to transparent    
                        if(gameBoard.gameBoardMatrix(row,column)==2)
                            cell.setFillColor(sf::Color::Black); // Set fill color to transparent    
                        window.draw(cell);
                    }
                }
                if(!playerCanRotate){
                    sf::Text textPlayerCanRotate("You can not rotate anymore !",font,40);
                    textPlayerCanRotate.setFillColor(sf::Color::Red);
                    textPlayerCanRotate.setPosition({static_cast<float>((windowWidth/2)-200), static_cast<float>(windowHight/2) });
                    window.draw(textPlayerCanRotate);
                }



                //draw next Piece into next piece area
                gameBoard.drawNextPiece( window, cell, cellSize, nextPiece, font);
                //draw next Piece into next piece area
                gameBoard.drawHoldPiece(window, cell, cellSize, holdedPiece, font);
                //draw score
                gameBoard.drawScore(score, window, cellSize, font);
                }
            }

        //draw soundon and of button
        if(shutBackgroundMusic){
            window.draw(spriteSoundOff);
        }else{
            window.draw(spriteSoundOn);
        }
        
        
        
        // end the current frame
        window.display();
    }


}