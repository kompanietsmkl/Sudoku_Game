
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <bitset>
#include <stdexcept>
#include <random>

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

    // Update bitsets when setting a number
    void updateBitsets(int row, int col, int num, bool setValue) {
        rowUsed[row][num-1] = setValue;
        colUsed[col][num-1] = setValue;
        boxUsed[(row/3)*3 + col/3][num-1] = setValue;
    }

    // Initialize bitsets based on current board state
    void initializeBitsets() {
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

public:
    SudokuBoard() : 
        board(SIZE, vector<int>(SIZE, 0)),
        solution(SIZE, vector<int>(SIZE, 0)),
        rowUsed(SIZE),
        colUsed(SIZE),
        boxUsed(SIZE) {}

    // Generate base grid with valid Sudoku solution
    void generateBaseGrid() {
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

    // Transpose the grid
    void transpose() {
        for(int i = 0; i < SIZE; i++) {
            for(int j = i + 1; j < SIZE; j++) {
                swap(board[i][j], board[j][i]);
            }
        }
        initializeBitsets();
    }

    // Swap two rows within the same block
    void swapRowsInBlock() {
        int block = rand() % SUBGRID_SIZE;
        int row1 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
        int row2 = block * SUBGRID_SIZE + rand() % SUBGRID_SIZE;
        if(row1 != row2) {
            swap(board[row1], board[row2]);
        }
        initializeBitsets();
    }

    // Swap two columns within the same block
    void swapColsInBlock() {
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

    // Swapping row blocks
    void swapRowBlocks() {
        int block1 = rand() % SUBGRID_SIZE;
        int block2 = rand() % SUBGRID_SIZE;
        if (block1 != block2) {
            for (int i = 0; i < SUBGRID_SIZE; i++) {
                swap(board[block1 * SUBGRID_SIZE + i], board[block2 * SUBGRID_SIZE + i]);
            }
        }
    }

    // Swapping column blocks
    void swapColBlocks() {
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

    // Randomize the grid while maintaining Sudoku rules
    void randomizeGrid() {
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

    // Remove numbers to create puzzle
    void removeNumbers(int numToRemove) {
        solution = board; // Save the solution
        vector<pair<int, int>> positions;
        
        // Create list of all positions
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                positions.push_back({i, j});
            }
        }
        
        // Shuffle positions
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(positions.begin(), positions.end(), gen);
        
        // Remove numbers
        for(int i = 0; i < numToRemove && i < positions.size(); i++) {
            int row = positions[i].first;
            int col = positions[i].second;
            board[row][col] = 0;
            updateBitsets(row, col, solution[row][col], false);
        }
    }

    // Check if move is valid
    bool isValidMove(int row, int col, int num) const {
        return !rowUsed[row][num-1] && 
               !colUsed[col][num-1] && 
               !boxUsed[(row/3)*3 + col/3][num-1];
    }

    // Make a move
    void makeMove(int row, int col, int num) {
        if(row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
            throw invalid_argument("Invalid input values");
        }
        
        if(board[row][col] != 0) {
            throw invalid_argument("Cell is already filled");
        }
        
        if(!isValidMove(row, col, num)) {
            throw invalid_argument("Invalid move");
        }

        board[row][col] = num;
        updateBitsets(row, col, num, true);
    }

    // Check if puzzle is solved
    bool isSolved() const {
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(board[i][j] != solution[i][j]) return false;
            }
        }
        return true;
    }

    // Get hint for next empty cell
    pair<int, int> getHint() const {
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(board[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // No empty cells
    }

    // Get solution value for a cell
    int getSolutionValue(int row, int col) const {
        return solution[row][col];
    }

    // Print the board
    void printBoard() const {
    // Print column numbers aligned with cells
    cout << "\n   "; // Extra space for alignment
    for(int j = 0; j < SIZE; j++) {
        cout << j + 1 << " ";
        if((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) cout << "  ";
    }
    cout << "\n   "; // Extra space for alignment
    for(int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
    cout << "\n";

    // Print rows
    for(int i = 0; i < SIZE; i++) {
        cout << i + 1 << "| ";
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == 0) cout << "_ ";
            else cout << board[i][j] << " ";
            if((j + 1) % SUBGRID_SIZE == 0 && j != SIZE - 1) cout << "| ";
        }
        cout << "|\n";
        if((i + 1) % SUBGRID_SIZE == 0 && i != SIZE - 1) {
            cout << "   "; // Extra space for alignment
            for(int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
            cout << "\n";
        }
    }
    cout << "   "; // Extra space for alignment
    for(int j = 0; j < SIZE * 2 + 3; j++) cout << "-";
    cout << "\n";
}
};

class SudokuGame {
private:
    SudokuBoard board;
    int difficulty;

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

public:
    SudokuGame() : difficulty(0) {}

    void start() {
        srand(time(0));
        clearScreen();

        cout << "\t\t\t<================================================================================>" << endl;
        cout << "\t\t\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
        cout << "\t\t\t|             Fill in the missing numbers to solve the puzzle.                   |" << endl;
        cout << "\t\t\t<================================================================================>" << endl;
            
        // Choose difficulty
        while(true) {
            cout << "Choose difficulty level:\n";
            cout << "1. Very Easy\n";
            cout << "2. Easy\n";
            cout << "3. Medium\n";
            cout << "4. Hard\n";
            difficulty = getValidInput("Your choice (1-4): ", 1, 4);  
            break;
        }

        // Create puzzle
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
        
        // Start game loop
        playGame();
    }

private:

    int getValidInput(const string& prompt, int min, int max) {
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

    void playGame() {
        while(true) {
            clearScreen();
            board.printBoard();

            cout << "\nChoose action:\n";
            cout << "[1] - Make a move\n";
            cout << "[2] - Get a hint\n";
            cout << "[3] - Start a new game\n";
            cout << "[4] - Exit\n";

            int choice = getValidInput("Your choice: ", 1, 4);

            try {
                switch(choice) {
                    case 1: {
                        int row, col, num;
                        cout << "Enter row (1-9), column (1-9), and number (1-9): ";
                        cin >> row >> col >> num;
                        
                        board.makeMove(row-1, col-1, num);
                        
                        if(board.isSolved()) {
                            clearScreen();
                            board.printBoard();
                            cout << "\t\t\t<================================================================================>" << endl;
                            cout << "\t\t\t|                                Congratulations!                                |\n";
                            cout << "\t\t\t|                           You have solved the puzzle!                          |\n";
                            cout << "\t\t\t<================================================================================>" << endl;

                            cout << "Press Enter to exit...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            return;
                        }
                        break;
                    }
                    case 2: {
                        auto hint = board.getHint();
                        if(hint.first != -1) {
                            cout << "Hint: Cell (" << hint.first + 1 << ", " << hint.second + 1 << ") should be " << board.getSolutionValue(hint.first, hint.second) << "\n";
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
};

int main() {
    try {
        SudokuGame game;
        game.start();
    }
    catch(const exception& e) {
        cerr << "Critical error: " << e.what() << endl;
        return 1;
    }

    return 0;
}