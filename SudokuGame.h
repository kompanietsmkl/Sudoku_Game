#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "SudokuBoard.h"
#include "Leaderboard.h"
#include <string>
using namespace std;

class SudokuGame {
private:
    SudokuBoard board;
    int difficulty;
    Leaderboard leaderboard;
    string playerName;

    void clearScreen();
    int getValidInput(const string& prompt, int min, int max);
    void playGame();

public:
    SudokuGame();
    void start();
};

#endif