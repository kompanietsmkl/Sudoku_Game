#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int SIZE = 9;
const int SUBGRIDSIZE = 3;


class Sudoku {
private:
    vector<vector<int>> board;
    vector<vector<int>> solution;

public:
    /*Here we are generating 9x9 base matrix with numbers 1-9 in each position. In order to make
     *every game unique and randomize number's order we use different distortion methods such as
     *transposition, swapping rows/cols in one block, swapping row/col blocks.*/

    Sudoku() : board(SIZE, vector<int>(SIZE, 0)) {}

    // Generating a base grid for further manipulation
    void generateBaseGrid() {
        int base[SIZE][SIZE] = {
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 1, 2, 3},
            {7, 8, 9, 1, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            {5, 6, 7, 8, 9, 1, 2, 3, 4},
            {8, 9, 1, 2, 3, 4, 5, 6, 7},
            {3, 4, 5, 6, 7, 8, 9, 1, 2},
            {6, 7, 8, 9, 1, 2, 3, 4, 5},
            {9, 1, 2, 3, 4, 5, 6, 7, 8}
        };

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                board[i][j] = base[i][j];
    }

    // Transpose
    void transpose() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = i + 1; j < SIZE; j++) {
                swap(board[i][j], board[j][i]);
            }
        }
    }

    // Rows swapping in one box
    void swapRowsInBlock() {
        int block = rand() % SUBGRIDSIZE;
        int row1 = block * SUBGRIDSIZE + rand() % SUBGRIDSIZE;
        int row2 = block * SUBGRIDSIZE + rand() % SUBGRIDSIZE;
        if (row1 != row2)
            swap(board[row1], board[row2]);
    }

    // Cols swapping in one box
    void swapColsInBlock() {
        int block = rand() % SUBGRIDSIZE;
        int col1 = block * SUBGRIDSIZE + rand() % SUBGRIDSIZE;
        int col2 = block * SUBGRIDSIZE + rand() % SUBGRIDSIZE;
        if (col1 != col2) {
            for (int i = 0; i < SIZE; i++)
                swap(board[i][col1], board[i][col2]);
        }
    }

    // Swapping row blocks
    void swapRowBlocks() {
        int block1 = rand() % SUBGRIDSIZE;
        int block2 = rand() % SUBGRIDSIZE;
        if (block1 != block2) {
            for (int i = 0; i < SUBGRIDSIZE; i++) {
                swap(board[block1 * SUBGRIDSIZE + i], board[block2 * SUBGRIDSIZE + i]);
            }
        }
    }

    // Swapping column blocks
    void swapColBlocks() {
        int block1 = rand() % SUBGRIDSIZE;
        int block2 = rand() % SUBGRIDSIZE;
        if (block1 != block2) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SUBGRIDSIZE; j++) {
                    swap(board[i][block1 * SUBGRIDSIZE + j], board[i][block2 * SUBGRIDSIZE + j]);
                }
            }
        }
    }

    // Choosing a random method of distortion
    void randomizeGrid() {
        int numShuffles = 10 + rand() % 10;  // Distortion q-ty
        for (int i = 0; i < numShuffles; i++) {
            int choice = rand() % 5;
            switch (choice) {
            case 0: transpose(); break;
            case 1: swapRowsInBlock(); break;
            case 2: swapColsInBlock(); break;
            case 3: swapRowBlocks(); break;
            case 4: swapColBlocks(); break;
            }
        }
    }

    // Removing numbers from matrix to create a puzzle
    void removeNumbers(int numToRemove) {
        int count = 0;
        while (count < numToRemove) {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            if (board[row][col] != 0) {
                board[row][col] = 0;
                count++;
            }
        }
    }

    // Creating a game with different difficulty levels depending on player's choice
    void createPuzzle(int difficulty) {
        generateBaseGrid();
        randomizeGrid();
        int numToRemove = 0;
        
        if (difficulty == 0) numToRemove = 0;  // Hidden option to see whole matrix
        else if (difficulty == 1) numToRemove = 45;  // Easy - removing 45/81 elements
        else if (difficulty == 2) numToRemove = 55;  // Medium - removing 55/81 elements
        else if (difficulty == 3) numToRemove = 65;  // Hard - removing 65/81 elements
        else if (difficulty == 4) numToRemove = 70;  // COOKED - removing 70/71 elements

        removeNumbers(numToRemove);
    }

    // Sudoku output
    void printBoard() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) cout << "_ ";
                else cout << board[i][j] << " ";
                if ((j + 1) % SUBGRIDSIZE == 0 && j != SIZE - 1) cout << "| ";
            }
            cout << endl;
            if ((i + 1) % SUBGRIDSIZE == 0 && i != SIZE - 1)
                cout << "------+-------+------" << endl;
        }
    }

    bool isValidMove(int row, int col, int num) {
        for (int i = 0; i < SIZE; i++) {
            if (board[row][i] == num || board[i][col] == num) return false;
        }

        int startRow = (row / SUBGRIDSIZE) * SUBGRIDSIZE;
        int startCol = (col / SUBGRIDSIZE) * SUBGRIDSIZE;
        for (int i = 0; i < SUBGRIDSIZE; i++) {
            for (int j = 0; j < SUBGRIDSIZE; j++) {
                if (board[startRow + i][startCol + j] == num) return false;
            }
        }
        return true;
    }

    bool isSolved() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0 || board[i][j] != solution[i][j]) return false;
            }
        }
        return true;
    }

    void playGame() {
    while (true) {
        system("cls");
        printBoard();  // Печать доски перед каждым вводом пользователя
        int row, col, num;
        cout << "Enter row (1-9), column (1-9), and number (1-9): ";
        cin >> row >> col >> num;

        row--;
        col--;

        // Проверка валидности ввода
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        if (board[row][col] != 0) {
            cout << "Cell is already filled. Please choose another cell.\n";
            continue;
        }

        // Проверка, можно ли вставить число в клетку
        if (!isValidMove(row, col, num)) {
            cout << "Invalid move. Number conflicts with row, column, or subgrid.\n";
            continue;
        }

        // Запись числа в клетку
        board[row][col] = num;

        // Печать обновленной доски после хода
        printBoard();

        // Проверка, есть ли еще пустые клетки на доске
        bool allFilled = true;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    allFilled = false;
                    break;
                }
            }
            if (!allFilled) break;
        }

        // Если все клетки заполнены, игра завершена
        if (allFilled) {
            cout << "Congratulations! You solved the puzzle!\n";
            break;
        }
    }

    // Пауза перед завершением программы
    cout << "Press Enter to exit...";
    cin.ignore();  // Очищаем буфер
    cin.get();     // Ожидаем нажатие Enter для завершения программы
}


};

int main() {
    srand(time(0));  // Initializing of random number generator
    Sudoku sudoku;
    int difficulty;

    cout << "\t\t\t<================================================================================>" << endl;
    cout << "\t\t\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
    cout << "\t\t\t|             Fill in the missing numbers to solve the puzzle.                   |" << endl;
    cout << "\t\t\t<================================================================================>" << endl;

    // Difficulty selection
    do{
        cout << "Choose difficulty (1 - Easy, 2 - Medium, 3 - Hard, 4 - COOKED): ";
        cin >> difficulty;

        if (difficulty >= 0 && difficulty <= 4) {
            break;
        }
        else
            cout << "Invalid Input! . Enter number from 1 to 4 \n";
    }while(true);
    
    sudoku.createPuzzle(difficulty);
    cout << "Generated puzzle:" << endl;
    
    sudoku.playGame();
    
    cout<< "Press Enter to exit..." << endl;
    cin.ignore();
    cin.get();

    return 0;
}