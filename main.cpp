#include "SudokuGame.h"
#include <iostream>
using namespace std;

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