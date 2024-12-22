#include "SudokuGame.h"
#include <iostream>

int main() {
    try {
        SudokuGame game;
        game.start();
    }
    catch(const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}