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
    vector<vector<bool>> isEditable;
    vector<bitset<9>> rowUsed;
    vector<bitset<9>> colUsed;
    vector<bitset<9>> boxUsed;

    void updateBitsets(int row, int col, int num, bool setValue);
    void initializeBitsets();

public:
    SudokuBoard();
    vector<vector<bool>> checkBoard() const;
    void generateBaseGrid();
    void transpose();
    void swapRowsInBlock();
    void swapColsInBlock();
    void swapRowBlocks();
    void swapColBlocks();
    void randomizeGrid();
    void removeNumbers(int numToRemove);
    void deleteMove(int row, int col);
    bool isValidMove(int row, int col, int num) const;
    void makeMove(int row, int col, int num);
    bool isSolved() const;
    bool isBoardFull() const;
    pair<int, int> getHint() const;
    int getSolutionValue(int row, int col) const;
    void printBoard() const;

    
};

#endif

