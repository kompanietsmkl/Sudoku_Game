#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "SudokuBoard.h"

class Solver {
private:
    static const int SIZE = 9;
    static const int SUBGRID_SIZE = 3;
    
    bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) const;
    bool findEmptyCell(const std::vector<std::vector<int>>& board, int& row, int& col) const;
    bool solveHelper(std::vector<std::vector<int>>& board, int row, int col);

public:
    bool solve(std::vector<std::vector<int>>& board);
    void inputPuzzle(std::vector<std::vector<int>>& board);
    void printBoard(const vector<vector<int>>& board) const;
};

#endif // SOLVER_H