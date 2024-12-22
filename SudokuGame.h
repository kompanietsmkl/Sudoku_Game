#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "SudokuBoard.h"
#include <string>

class SudokuGame {
private:
    SudokuBoard board;
    int difficulty;

    void clearScreen();
    int getValidInput(const std::string& prompt, int min, int max);
    void playGame();

public:
    SudokuGame();
    void start();
};

#endif