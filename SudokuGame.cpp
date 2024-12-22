#include "SudokuGame.h"
#include <iostream>
#include <limits>
#include <cstdlib>

SudokuGame::SudokuGame() : difficulty(0) {}

void SudokuGame::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int SudokuGame::getValidInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Number must be between " << min << " and " << max << "!\n";
    }
}

void SudokuGame::start() {
    srand(time(0));
    clearScreen();

    std::cout << "\t\t\t<================================================================================>" << std::endl;
    std::cout << "\t\t\t|                        WELCOME TO SUDOKU Game!                                 |" << std::endl;
    std::cout << "\t\t\t|             Fill in the missing numbers to solve the puzzle.                   |" << std::endl;
    std::cout << "\t\t\t<================================================================================>" << std::endl;
        
    while(true) {
        std::cout << "Choose difficulty level:\n";
        std::cout << "1. Very Easy\n";
        std::cout << "2. Easy\n";
        std::cout << "3. Medium\n";
        std::cout << "4. Hard\n";
        difficulty = getValidInput("Your choice (1-4): ", 1, 4);
        break;
    }

    board = SudokuBoard();
    board.generateBaseGrid();
    board.randomizeGrid();
    
    int numToRemove;
    switch(difficulty) {
        case 1: numToRemove = 4; break;
        case 2: numToRemove = 45; break;
        case 3: numToRemove = 55; break;
        case 4: numToRemove = 65; break;
    }
    
    board.removeNumbers(numToRemove);
    playGame();
}

void SudokuGame::playGame() {
    while(true) {
        clearScreen();
        board.printBoard();

        std::cout << "\nChoose action:\n";
        std::cout << "[1] - Make a move\n";
        std::cout << "[2] - Get a hint\n";
        std::cout << "[3] - Start a new game\n";
        std::cout << "[4] - Exit\n";

        int choice = getValidInput("Your choice: ", 1, 4);

        try {
            switch(choice) {
                case 1: {
                    int row, col, num;
                    while (true) {
                        std::cout << "Enter row (1-9), column (1-9), and number (1-9): ";
                        if (!(std::cin >> row >> col >> num)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input! Please enter three numbers.\n";
                            continue;
                        }

                        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Numbers must be between 1 and 9!\n";
                            continue;
                        }

                        try {
                            board.makeMove(row-1, col-1, num);
                            break; // Выходим из цикла только если ход успешен
                        }
                        catch(const std::exception& e) {
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Error: " << e.what() << "\n";
                            continue;
                        }
                    }
                    
                    if(board.isSolved()) {
                        clearScreen();
                        board.printBoard();
                        std::cout << "\t\t\t<================================================================================>" << std::endl;
                        std::cout << "\t\t\t|                                Congratulations!                                |" << std::endl;
                        std::cout << "\t\t\t|                           You have solved the puzzle!                         |" << std::endl;
                        std::cout << "\t\t\t<================================================================================>" << std::endl;

                        std::cout << "Press Enter to exit...";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        return;
                    }
                    break;
                }
                case 2: {
                    auto hint = board.getHint();
                    if(hint.first != -1) {
                        std::cout << "Hint: Cell (" << hint.first + 1 << ", " << hint.second + 1 << ") should be " << board.getSolutionValue(hint.first, hint.second) << "\n";
                    } else {
                        std::cout << "No empty cells left!\n";
                    }
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                case 3: {
                    std::cout << "\nStarting new game...\n";
                    start();
                    return;
                }
                case 4: {
                    std::cout << "\nThank you for playing!\n";
                    return;
                }
                default: {
                    std::cout << "Invalid choice!\n";
                }
            }
        }
        catch(const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}