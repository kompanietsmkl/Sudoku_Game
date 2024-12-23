#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <vector>
#include <bitset>
#include <utility>
using namespace std;

class SudokuBoard {
private:
    static const int SIZE = 9;
    static const int SUBGRID_SIZE = 3;
    
    vector<vector<int>> board;
    vector<vector<int>> solution;
    vector<bitset<9>> rowUsed;
    vector<bitset<9>> colUsed;
    vector<bitset<9>> boxUsed;

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
    pair<int, int> getHint() const;
    int getSolutionValue(int row, int col) const;
    void printBoard() const;
};

#endif

