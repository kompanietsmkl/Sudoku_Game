#include "SudokuBoard.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>
using namespace std;

// Constructor: Initializes the Sudoku board and supporting structures.
SudokuBoard::SudokuBoard() : 
    board(SIZE, vector<int>(SIZE, 0)),
    solution(SIZE, vector<int>(SIZE, 0)),
    rowUsed(SIZE),
    colUsed(SIZE),
    boxUsed(SIZE) {}

// Updates bitsets used to track which numbers are already present in rows, columns, and boxes.
void SudokuBoard::updateBitsets(int row, int col, int num, bool setValue) {
    rowUsed[row][num-1] = setValue;
    colUsed[col][num-1] = setValue;
    boxUsed[(row/3)*3 + col/3][num-1] = setValue;
}

// Initializes bitsets based on the current board state.
void SudokuBoard::initializeBitsets() {
    rowUsed = vector<bitset<9>>(9, bitset<9>());
    colUsed = vector<bitset<9>>(9, bitset<9>());
    boxUsed = vector<bitset<9>>(9, bitset<9>());
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] != 0) {
                updateBitsets(i, j, board[i][j], true);
            }
        }
    }
}

// Fills the board with a base valid Sudoku grid.
void SudokuBoard::generateBaseGrid() {
    const int base[SIZE][SIZE] = {
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

    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            board[i][j] = base[i][j];
            
    initializeBitsets();
}

// Transposes the board (swaps rows with columns) to create a new variation.
void SudokuBoard::transpose() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = i + 1; j < SIZE; j++) {
            swap(board[i][j], board[j][i]);
        }
    }
    initializeBitsets();
}

// Swaps two rows within the same subgrid.
void SudokuBoard::swapRowsInBlock() {
    int block = rand() % SUBGRID_SIZE;
    int row1 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    int row2 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    if(row1 != row2) {
        swap(board[row1], board[row2]);
    }
    initializeBitsets();
}

// Swaps two columns within the same subgrid.
void SudokuBoard::swapColsInBlock() {
    int block = rand() % SUBGRID_SIZE;
    int col1 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    int col2 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    if(col1 != col2) {
        for(int i = 0; i < SIZE; i++) {
            swap(board[i][col1], board[i][col2]);
        }
    }
    initializeBitsets();
}

// Swaps entire row blocks to create a new variation of the grid.
void SudokuBoard::swapRowBlocks() {
    int block1 = rand() % SUBGRID_SIZE;
    int block2 = rand() % SUBGRID_SIZE;
    if (block1 != block2) {
        for (int i = 0; i < SUBGRID_SIZE; i++) {
            swap(board[block1 * SUBGRID_SIZE + i], board[block2 * SUBGRID_SIZE + i]);
        }
    }
}

// Swaps entire column blocks to create a new variation of the grid.
void SudokuBoard::swapColBlocks() {
    int block1 = rand() % SUBGRID_SIZE;
    int block2 = rand() % SUBGRID_SIZE;
    if (block1 != block2) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SUBGRID_SIZE; j++) {
                swap(board[i][block1 * SUBGRID_SIZE + j], board[i][block2 * SUBGRID_SIZE + j]);
            }
        }
    }
}

// Randomizes the grid by performing a series of transformations.
void SudokuBoard::randomizeGrid() {
    int numShuffles = 10 + rand() % 10;
    for(int i = 0; i < numShuffles; i++) {
        switch(rand() % 5) {
            case 0: transpose(); break;
            case 1: swapRowsInBlock(); break;
            case 2: swapColsInBlock(); break;
            case 3: swapRowBlocks(); break;
            case 4: swapColBlocks(); break;
        }
    }
}

// Removes numbers from the grid to create the puzzle, ensuring they can be re-added by the player.
void SudokuBoard::removeNumbers(int numToRemove) {
    solution = board; // Store the solution for validation.
    vector<pair<int, int>> positions;
    isEditable = vector<vector<bool>>(SIZE, vector<bool>(SIZE, false));
    
    // Collect all positions on the board.
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            positions.push_back({i, j});
        }
    }
    
    random_device rd;
    mt19937 gen(rd());
    shuffle(positions.begin(), positions.end(), gen); // Randomize positions.
    
    for(int i = 0; i < numToRemove && i < positions.size(); i++) {
        int row = positions[i].first;
        int col = positions[i].second;
        board[row][col] = 0; // Clear the cell.
        isEditable[row][col] = true; // Mark it as editable.
        updateBitsets(row, col, solution[row][col], false);
    }
}

// Checks if a move is valid (i.e., does not conflict with existing numbers).
bool SudokuBoard::isValidMove(int row, int col, int num) const {
    return !rowUsed[row][num-1] && 
           !colUsed[col][num-1] && 
           !boxUsed[(row/3)*3 + col/3][num-1];
}

// Makes a move by placing a number on the board if it's valid.
void SudokuBoard::makeMove(int row, int col, int num) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
        throw invalid_argument("Invalid input values");
    }

    if (!isEditable[row][col]) {
        throw invalid_argument("This cell cannot be changed");
    }

    // If the cell already contains a value, remove it before placing the new one.
    if (board[row][col] != 0) {
        updateBitsets(row, col, board[row][col], false);
    }

    if (!isValidMove(row, col, num)) {
        // Revert the previous value if the new one is invalid.
        if (board[row][col] != 0) {
            updateBitsets(row, col, board[row][col], true);
        }
        throw invalid_argument("Invalid move: number conflicts with row, column, or block");
    }

    board[row][col] = num;
    updateBitsets(row, col, num, true);
}

// Deletes a number from the board, making the cell empty.
void SudokuBoard::deleteMove(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        throw invalid_argument("Invalid input values");
    }

    if (!isEditable[row][col]) {
        throw invalid_argument("This cell cannot be changed");
    }

    if (board[row][col] == 0) {
        throw invalid_argument("Cell is already empty");
    }

    updateBitsets(row, col, board[row][col], false);
    board[row][col] = 0; // Clear the cell.
}

// Checks if the board is solved by comparing it with the solution.
bool SudokuBoard::isSolved() const {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] != solution[i][j]) return false;
        }
    }
    return true;
}

// Provides a hint by identifying the first empty cell and its solution.
pair<int, int> SudokuBoard::getHint() const {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// Retrieves the solution value for a specific cell.
int SudokuBoard::getSolutionValue(int row, int col) const {
    return solution[row][col];
}

// Checks if the board is fully filled.
bool SudokuBoard::isBoardFull() const {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == 0) return false;
        }
    }
    return true;
}

// Compares the current board with the solution to identify errors.
vector<vector<bool>> SudokuBoard::checkBoard() const {
    vector<vector<bool>> errors(SIZE, vector<bool>(SIZE, false));
    
    if (isBoardFull()) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (isEditable[i][j] && board[i][j] != solution[i][j]) {
                    errors[i][j] = true;
                }
            }
        }
    }
    return errors;
}

// Displays the Sudoku board in a formatted and color-coded way.
void SudokuBoard::printBoard() const {
    const string RESET_COLOR = "\033[0m";
    const string BLUE_COLOR = "\033[96m";
    const string WHITE_COLOR = "\033[37m";
    const string RED_COLOR = "\033[31m";

    vector<vector<bool>> errors = checkBoard();
    bool showErrors = isBoardFull();

    cout << "\n   ";
    for (int j = 0; j < SIZE; j++) {
        cout << j + 1 << " ";
        if ((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) cout << "  ";
    }
    cout << "\n   ";
    for (int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
    cout << "\n";

    for (int i = 0; i < SIZE; i++) {
        cout << i + 1 << "| ";
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                cout << "_ ";
            } else {
                if (isEditable[i][j]) {
                    if (errors[i][j]) {
                        cout << RED_COLOR;
                    } else {
                        cout << BLUE_COLOR;
                    }
                } else {
                    cout << WHITE_COLOR;
                }

                cout << board[i][j] << RESET_COLOR << " ";
            }
            if ((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) cout << "| ";
        }
        cout << "|\n";
        if ((i + 1) % SUBGRID_SIZE == 0 && i != SIZE - 1) {
            cout << "   ";
            for (int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
            cout << "\n";
        }
    }
    cout << "   ";
    for (int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
    cout << "\n";
}
