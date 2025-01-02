#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "SudokuBoard.h"
#include "Leaderboard.h"
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

class SudokuGame {
private:
    SudokuBoard board;
    int difficulty;
    Leaderboard leaderboard;
    string playerName;
    int score;

    time_point<system_clock> startTime;
    int elapsedSeconds;
    bool timerRunning;

    void clearScreen();
    int getValidInput(const string& prompt, int min, int max);
    void playGame();
    void updateTimer();
    void startTimer();
    string formatTime(int seconds);

public:
    SudokuGame();
    void start();
};

#endif