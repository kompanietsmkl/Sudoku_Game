#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <vector>
#include <bitset>
#include <utility>

class SudokuBoard {
private:
    static const int SIZE = 9;
    static const int SUBGRID_SIZE = 3;
    
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> solution;
    std::vector<std::bitset<9>> rowUsed;
    std::vector<std::bitset<9>> colUsed;
    std::vector<std::bitset<9>> boxUsed;

    void updateBitsets(int row, int col, int num, bool setValue);
    void initializeBitsets();

public:
    SudokuBoard();
    void generateBaseGrid();
    void transpose();
    void swapRowsInBlock();
    void swapColsInBlock();
    void swapRowBlocks();
    void swapColBlocks();
    void randomizeGrid();
    void removeNumbers(int numToRemove);
    bool isValidMove(int row, int col, int num) const;
    void makeMove(int row, int col, int num);
    bool isSolved() const;
    std::pair<int, int> getHint() const;
    int getSolutionValue(int row, int col) const;
    void printBoard() const;
};

#endif

