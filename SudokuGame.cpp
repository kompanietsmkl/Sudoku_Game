#include "SudokuGame.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <thread>
#include <iomanip>
using namespace std;

// Constructor: Initializes the game, sets default values for leaderboard, difficulty, and score.
SudokuGame::SudokuGame() 
    : leaderboard("leaderboard.txt"), 
    difficulty(0), 
    score(0),
    elapsedSeconds(0),
    timerRunning(false) {}

// Clears the screen based on the operating system.
void SudokuGame::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void SudokuGame::startTimer() {
    startTime = system_clock::now();
    timerRunning = true;
    elapsedSeconds = 0;
}

void SudokuGame::updateTimer() {
    if (timerRunning) {
        auto currentTime = system_clock::now();
        elapsedSeconds = duration_cast<seconds>(currentTime - startTime).count();
    }
}

string SudokuGame::formatTime(int seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    stringstream ss;
    ss << setfill('0') << setw(2) << hours << ":"
       << setfill('0') << setw(2) << minutes << ":"
       << setfill('0') << setw(2) << secs;
    return ss.str();
}

// Prompts the user for a valid integer input within a specified range.
int SudokuGame::getValidInput(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Number must be between " << min << " and " << max << "!\n";
    }
}

// Starts a new game by setting up the board and difficulty level.
void SudokuGame::start() {
    srand(time(0)); // Seed for random number generation.
    clearScreen();

    startTime = system_clock::now();
    timerRunning = true;

    // Display the welcome message.
    cout << "\t\t\t<================================================================================>" << endl;
    cout << "\t\t\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
    cout << "\t\t\t|             Fill in the missing numbers to solve the puzzle.                   |" << endl;
    cout << "\t\t\t<================================================================================>" << endl;

    // Prompt the user for their nickname.
    do {
        cout << "ENTER YOUR NICKNAME (3-20 chars): ";
        getline(cin, playerName);
    } while (playerName.length() < 3 || playerName.length() > 20);

    // Select the difficulty level.
    while (true) {
        cout << "Choose difficulty level:\n";
        cout << "1. Very Easy\n";
        cout << "2. Easy\n";
        cout << "3. Medium\n";
        cout << "4. Hard\n";
        difficulty = getValidInput("Your choice (1-4): ", 1, 4);
        break;
    }

    // Initialize the board and adjust the number of cells to remove based on difficulty.
    board = SudokuBoard();
    board.generateBaseGrid();
    board.randomizeGrid();

    int numToRemove;
    switch (difficulty) {
        case 1: numToRemove = 4; break;
        case 2: numToRemove = 40; break;
        case 3: numToRemove = 50; break;
        case 4: numToRemove = 60; break;
    }

    score = difficulty * 100; // Base score based on difficulty.
    board.removeNumbers(numToRemove); // Remove numbers to create the puzzle.

    startTimer();
    playGame();
}

// Main game loop for playing the Sudoku game.
void SudokuGame::playGame() {
    while (true) {
        clearScreen();
        updateTimer();

        if (timerRunning) {
            cout << "\nTime: " << formatTime(elapsedSeconds) << "\n";
        }

        board.printBoard();

        // Display available actions.
        cout << "\nChoose action:\n";
        cout << "[1] - Make a move\n";
        cout << "[2] - Get a hint\n";
        cout << "[3] - Start a new game\n";
        cout << "[4] - Delete a cell\n";
        cout << "[5] - Leaderboard\n";
        cout << "[6] - Exit\n";

        int choice = getValidInput("Your choice: ", 1, 6);

        try {
            switch (choice) {
                case 1: { // Make a move.
                    int row, col, num;
                    while (true) {
                        cout << "Enter row (1-9), column (1-9), and number (1-9): ";
                        if (!(cin >> row >> col >> num)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter three numbers.\n";
                            continue;
                        }

                        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Numbers must be between 1 and 9!\n";
                            continue;
                        }

                        try {
                            board.makeMove(row - 1, col - 1, num);
                            break; // Exit the loop on successful move.
                        } catch (const exception& e) {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Error: " << e.what() << "\n";
                            continue;
                        }
                    }

                    // Check if the puzzle is solved.
                    if (board.isSolved()) {
                        timerRunning = false;
                        clearScreen();
                        updateTimer();

                        board.printBoard();
                        cout << "\nFinal Time: " << formatTime(elapsedSeconds) << "\n";

                        cout << "\t\t\t<================================================================================>" << endl;
                        cout << "\t\t\t|                                Congratulations!                                |" << endl;
                        cout << "\t\t\t|                           You have solved the puzzle!                          |" << endl;
                        cout << "\t\t\t<================================================================================>" << endl;

                        int timeBonus = max(0, 300 - elapsedSeconds)/10; // Bonus fot solving under 5 minutes
                        score += timeBonus;

                        cout << "Time Bonus: +" << (timeBonus) << " points\n";
                        cout << "Final Score: " << score << " points\n";

                        leaderboard.addResult(playerName, score);
                        leaderboard.display();

                        cout << "Press Enter to exit...";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();
                        return;
                    }
                    break;
                }
                case 2: { // Get a hint.
                    auto hint = board.getHint();
                    if (hint.first != -1) {
                        cout << "Hint: Cell (" << hint.first + 1 << ", " << hint.second + 1 << ") should be " << board.getSolutionValue(hint.first, hint.second) << "\n";
                        score -= 5; // Deduct points for using a hint.
                    } else {
                        cout << "No empty cells left!\n";
                    }
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                case 3: { // Start a new game.
                    cout << "\nStarting new game...\n";
                    timerRunning = false;
                    start();
                    return;
                }
                case 4: { // Delete a cell.
                    int row, col, num;
                    cout << "Enter row (1-9) and column (1-9): ";
                    cin >> row >> col;
                    board.deleteMove(row - 1, col - 1);
                    break;
                }
                case 5: { // Display leaderboard.
                    cout << "Leaderboard:\n";
                    leaderboard.display();
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                case 6: { // Exit the game.
                    cout << "\nThank you for playing!\n";
                    updateTimer();
                    cout << "\nTime: " << formatTime(elapsedSeconds) << "\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                default: {
                    cout << "Invalid choice!\n";
                }
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
