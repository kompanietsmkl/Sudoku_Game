#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "SudokuBoard.h"
#include "Leaderboard.h"
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
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
    atomic<int> elapsedSeconds;
    atomic<bool> timerRunning;
    thread timerThread;
    mutex consoleMutex;

    void clearScreen();
    int getValidInput(const string& prompt, int min, int max);
    void playGame();
    void startTimer();
    void stopTimer();
    string formatTime(int seconds);
    void timerLoop();
    void printGameState();

public:
    SudokuGame();
    ~SudokuGame();
    void start();
};

#endif