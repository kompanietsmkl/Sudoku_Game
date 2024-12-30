#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Leaderboard::Leaderboard(const string& file) : filename(file) {
    load();
}

void Leaderboard::load() {
    scores.clear();
    ifstream file(filename);
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            scores.emplace_back(name, score);
        }
        file.close();
    }
}

void Leaderboard::save() {
    ofstream file(filename, ios::trunc);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.first << " " << entry.second << "\n";
        }
        file.close();
    }
}

void Leaderboard::addResult(const string& playerName, int score) {

    auto it = find_if(scores.begin(), scores.end(), [&](const auto& entry) {
        return entry.first == playerName;
    });

    if (it != scores.end()) {
        
        if (score > it->second) {
            it->second = score;
        }
    } else {
        scores.emplace_back(playerName, score);
    }

    sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return b.second > a.second;
    });
    save();
}

void Leaderboard::display() const {
    cout << "\n--- Leaderboard ---\n";
    for (const auto& entry : scores) {
        cout << entry.first << ": " << entry.second << "\n";
    }
    cout << "-------------------\n";
}
