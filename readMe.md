#Tetris Game

##Overview
This is a simple tetris game written in cpp using SFML library.

##Features
- Classic tetris gameplay with extra features like shrinking board, screen blackout etc.
- User-friendly graphical interface
- Scoring system basen on cleared lines
- Basic Theme Manager

##Requirements
- C++ compiler (supporting C++17 or later)
- Cmake 

## How to compile 
- Ensure that you have installed C++ compiler on your system
- Clone this repository to your PC
- Create a build folder in project folder and go into it
- write following commands to build SFML and project and run
- If you are using macOS
      - cmake ..
      - make
      - cd bin
      - ./tetris
- If your are on windows
      - cmake and nmake should be installed on your system
      - you can use microsoft visual studio comunity for ease
      - after you installed microsoft visual studio community build project and run code
That is it. Enjoy tetris game.

##Note:
  - You may adjust paths in code because based on compilers and OS the folder that is tetris.exe in may change. generally it is in build -> bin directory.
