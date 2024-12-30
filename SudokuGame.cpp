#include "SudokuGame.h"
#include <iostream>
#include <limits>
#include <cstdlib>
using namespace std;

SudokuGame::SudokuGame() : leaderboard("leaderboard.txt"), difficulty(0),score(0) {}

void SudokuGame::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/*************  ✨ Codeium Command ⭐  *************/
/**
 * Prompts the user for input and validates it.
 * 
 * Displays the given prompt message and reads an integer input from the user.
 * Ensures that the input is a valid integer within the specified range [min, max].
 * If the input is invalid, it prompts the user again until a valid input is entered.
 * 
 * @param prompt A string message to display when asking for input.
 * @param min The minimum acceptable integer value.
 * @param max The maximum acceptable integer value.
 * @return A valid integer input from the user within the range [min, max].
 */

/******  8d25c9ae-2ffb-41bc-93fb-7b6c1ed67bed  *******/
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

void SudokuGame::start() {
    srand(time(0));
    clearScreen();

    cout << "\t\t\t<================================================================================>" << endl;
    cout << "\t\t\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
    cout << "\t\t\t|             Fill in the missing numbers to solve the puzzle.                   |" << endl;
    cout << "\t\t\t<================================================================================>" << endl;

    cout << "Enter your nickname: ";
    cin >> playerName;    

    while(true) {
        cout << "Choose difficulty level:\n";
        cout << "1. Very Easy\n";
        cout << "2. Easy\n";
        cout << "3. Medium\n";
        cout << "4. Hard\n";
        difficulty = getValidInput("Your choice (1-4): ", 1, 4);
        break;
    }

    board = SudokuBoard();
    board.generateBaseGrid();
    board.randomizeGrid();
    
    int numToRemove;
    switch(difficulty) {
        case 1: numToRemove = 4; break;
        case 2: numToRemove = 40; break;
        case 3: numToRemove = 50; break;
        case 4: numToRemove = 60; break;
    }
    
    score = difficulty * 100;
    
    board.removeNumbers(numToRemove);
    playGame();
}

void SudokuGame::playGame() {
    while(true) {
        clearScreen();
        board.printBoard();

        cout << "\nChoose action:\n";
        cout << "[1] - Make a move\n";
        cout << "[2] - Get a hint\n";
        cout << "[3] - Start a new game\n";
        cout << "[4] - Exit\n";
        cout << "[5] - Leaderboard\n";

        int choice = getValidInput("Your choice: ", 1, 5);

        try {
            switch(choice) {
                case 1: {
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
                            board.makeMove(row-1, col-1, num);
                            break; // Выходим из цикла только если ход успешен
                        }
                        catch(const exception& e) {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Error: " << e.what() << "\n";
                            continue;
                        }
                    }
                    
                    if(board.isSolved()) {
                        clearScreen();
                        board.printBoard();
                        cout << "\t\t\t<================================================================================>" << endl;
                        cout << "\t\t\t|                                Congratulations!                                |" << endl;
                        cout << "\t\t\t|                           You have solved the puzzle!                          |" << endl;
                        cout << "\t\t\t<================================================================================>" << endl;
                        leaderboard.addResult(playerName, score); // Example scoring
                        leaderboard.display();

                        cout << "Press Enter to exit...";
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                        cin.get();
                        return;
                    }
                    break;
                }
                case 2: {
                    auto hint = board.getHint();
                    if(hint.first != -1) {
                        cout << "Hint: Cell (" << hint.first + 1 << ", " << hint.second + 1 << ") should be " << board.getSolutionValue(hint.first, hint.second) << "\n";
                        score -= 5;
                    } else {
                        cout << "No empty cells left!\n";
                    }
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                case 3: {
                    cout << "\nStarting new game...\n";
                    start();
                    return;
                }
                case 4: {
                    cout << "\nThank you for playing!\n";
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cin.get();
                    return;
                }
                default: {
                    cout << "Invalid choice!\n";
                }
            }
        }
        catch(const exception& e) {
            cout << "Error: " << e.what() << "\n";
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}