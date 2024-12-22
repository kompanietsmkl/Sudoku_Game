#include "SudokuBoard.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>

SudokuBoard::SudokuBoard() : 
    board(SIZE, std::vector<int>(SIZE, 0)),
    solution(SIZE, std::vector<int>(SIZE, 0)),
    rowUsed(SIZE),
    colUsed(SIZE),
    boxUsed(SIZE) {}

void SudokuBoard::updateBitsets(int row, int col, int num, bool setValue) {
    rowUsed[row][num-1] = setValue;
    colUsed[col][num-1] = setValue;
    boxUsed[(row/3)*3 + col/3][num-1] = setValue;
}

void SudokuBoard::initializeBitsets() {
    rowUsed = std::vector<std::bitset<9>>(9, std::bitset<9>());
    colUsed = std::vector<std::bitset<9>>(9, std::bitset<9>());
    boxUsed = std::vector<std::bitset<9>>(9, std::bitset<9>());
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] != 0) {
                updateBitsets(i, j, board[i][j], true);
            }
        }
    }
}

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

void SudokuBoard::transpose() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = i + 1; j < SIZE; j++) {
            std::swap(board[i][j], board[j][i]);
        }
    }
    initializeBitsets();
}

void SudokuBoard::swapRowsInBlock() {
    int block = rand() % SUBGRID_SIZE;
    int row1 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    int row2 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    if(row1 != row2) {
        std::swap(board[row1], board[row2]);
    }
    initializeBitsets();
}

void SudokuBoard::swapColsInBlock() {
    int block = rand() % SUBGRID_SIZE;
    int col1 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    int col2 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
    if(col1 != col2) {
        for(int i = 0; i < SIZE; i++) {
            std::swap(board[i][col1], board[i][col2]);
        }
    }
    initializeBitsets();
}

void SudokuBoard::swapRowBlocks() {
    int block1 = rand() % SUBGRID_SIZE;
    int block2 = rand() % SUBGRID_SIZE;
    if (block1 != block2) {
        for (int i = 0; i < SUBGRID_SIZE; i++) {
            std::swap(board[block1 * SUBGRID_SIZE + i], board[block2 * SUBGRID_SIZE + i]);
        }
    }
}

void SudokuBoard::swapColBlocks() {
    int block1 = rand() % SUBGRID_SIZE;
    int block2 = rand() % SUBGRID_SIZE;
    if (block1 != block2) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SUBGRID_SIZE; j++) {
                std::swap(board[i][block1 * SUBGRID_SIZE + j], board[i][block2 * SUBGRID_SIZE + j]);
            }
        }
    }
}

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

void SudokuBoard::removeNumbers(int numToRemove) {
    solution = board;
    std::vector<std::pair<int, int>> positions;
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            positions.push_back({i, j});
        }
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);
    
    for(int i = 0; i < numToRemove && i < positions.size(); i++) {
        int row = positions[i].first;
        int col = positions[i].second;
        board[row][col] = 0;
        updateBitsets(row, col, solution[row][col], false);
    }
}

bool SudokuBoard::isValidMove(int row, int col, int num) const {
    return !rowUsed[row][num-1] && 
           !colUsed[col][num-1] && 
           !boxUsed[(row/3)*3 + col/3][num-1];
}

void SudokuBoard::makeMove(int row, int col, int num) {
    if(row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
        throw std::invalid_argument("Invalid input values");
    }
    
    if(board[row][col] != 0) {
        throw std::invalid_argument("Cell is already filled");
    }
    
    if(!isValidMove(row, col, num)) {
        throw std::invalid_argument("Invalid move");
    }

    board[row][col] = num;
    updateBitsets(row, col, num, true);
}

bool SudokuBoard::isSolved() const {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] != solution[i][j]) return false;
        }
    }
    return true;
}

std::pair<int, int> SudokuBoard::getHint() const {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

int SudokuBoard::getSolutionValue(int row, int col) const {
    return solution[row][col];
}

void SudokuBoard::printBoard() const {
    std::cout << "\n   ";
    for(int j = 0; j < SIZE; j++) {
        std::cout << j + 1 << " ";
        if((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) std::cout << "  ";
    }
    std::cout << "\n   ";
    for(int j = 0; j < SIZE * 2 + 3; j++) std::cout << "-";
    std::cout << "\n";

    for(int i = 0; i < SIZE; i++) {
        std::cout << i + 1 << "| ";
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == 0) std::cout << "_ ";
            else std::cout << board[i][j] << " ";
            if((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) std::cout << "| ";
        }
        std::cout << "|\n";
        if((i + 1) % SUBGRID_SIZE == 0 && i != SIZE - 1) {
            std::cout << "   ";
            for(int j = 0; j < SIZE * 2 + 3; j++) std::cout << "-";
            std::cout << "\n";
        }
    }
    std::cout << "   ";
    for(int j = 0; j < SIZE * 2 + 3; j++) std::cout << "-";
    std::cout << "\n";
}