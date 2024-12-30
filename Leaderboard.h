#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class Leaderboard {
private:
    string filename;
    vector<pair<string, int>> scores;

    void load(); 
    void save(); 

public:
    Leaderboard(const string& file);
    void addResult(const string& playerName, int score);
    void display() const;
};

#endif
