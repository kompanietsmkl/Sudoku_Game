#include "Solver.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <unordered_set>
using namespace std;

bool Solver::isValid(const vector<vector<int>>& board, int row, int col, int num) const {
    // Check row
    for(int j = 0; j < SIZE; j++) {
        if(board[row][j] == num) return false;
    }
    
    // Check column
    for(int i = 0; i < SIZE; i++) {
        if(board[i][col] == num) return false;
    }
    
    // Check 3x3 box
    int boxRow = row - row % SUBGRID_SIZE;
    int boxCol = col - col % SUBGRID_SIZE;
    for(int i = 0; i < SUBGRID_SIZE; i++) {
        for(int j = 0; j < SUBGRID_SIZE; j++) {
            if(board[boxRow + i][boxCol + j] == num) return false;
        }
    }
    
    return true;
}

bool Solver::findEmptyCell(const vector<vector<int>>& board, int& row, int& col) const {
    for(row = 0; row < SIZE; row++) {
        for(col = 0; col < SIZE; col++) {
            if(board[row][col] == 0) return true;
        }
    }
    return false;
}

bool Solver::solveHelper(vector<vector<int>>& board, int row, int col) {
    // If we've filled all cells, we're done
    if(row == SIZE - 1 && col == SIZE) return true;
    
    // Move to next row if we've reached end of current row
    if(col == SIZE) {
        row++;
        col = 0;
    }
    
    // Skip if cell is already filled
    if(board[row][col] != 0) {
        return solveHelper(board, row, col + 1);
    }
    
    // Try digits 1-9 in current cell
    for(int num = 1; num <= 9; num++) {
        if(isValid(board, row, col, num)) {
            board[row][col] = num;
            
            if(solveHelper(board, row, col + 1)) return true;
            
            // If placing num didn't lead to a solution, backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

bool Solver::solve(vector<vector<int>>& board) {
    int row = 0, col = 0;
    return solveHelper(board, row, col);
}

void Solver::inputPuzzle(vector<vector<int>>& board) {
    cout << "\nEnter the Sudoku puzzle, row by row (use 0 for empty cells):\n";
    cout << "Example format for each row: 5 3 0 0 7 0 0 0 0\n\n";
    
    // Инициализация двумерного вектора
    board = vector<vector<int>>(SIZE, vector<int>(SIZE));
    
    for (int i = 0; i < SIZE; i++) {
        while (true) {
            cout << "Enter row " << i + 1 << ": ";
            string rowInput;
            getline(cin, rowInput);
            stringstream ss(rowInput);
            vector<int> row;
            unordered_set<int> seenNumbers;
            vector<int> duplicates;
            int value;
            
            while (ss >> value) {
                if (value < 0 || value > 9) {
                    cout << "Invalid input! Please enter numbers between 0-9.\n";
                    row.clear();
                    break;
                }
                if (value != 0 && !seenNumbers.insert(value).second) {
                    duplicates.push_back(value);
                }
                row.push_back(value);
            }
            
            // Проверка на количество значений в строке
            if (row.size() != SIZE) {
                cout << "Invalid input! Each row must contain exactly 9 numbers.\n";
                continue;
            }
            
            if (!duplicates.empty()) {
                cout << "Invalid input! The following numbers are repeated in the row: ";
                for (size_t j = 0; j < duplicates.size(); ++j) {
                    cout << duplicates[j] << (j < duplicates.size() - 1 ? ", " : "\n");
                }
                continue;
            }

            board[i] = row;
            break;
        }
    }
}

void Solver::printBoard(const vector<vector<int>>& board) const {
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
                cout << board[i][j] << " ";
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