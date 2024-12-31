#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// Constructor: Initializes the leaderboard with a file and loads data from it.
Leaderboard::Leaderboard(const string& file) : filename(file) {
    load();
}

// Loads scores from the file into memory.
void Leaderboard::load() {
    scores.clear(); // Ensure scores are empty before loading.
    ifstream file(filename);
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            // Read player names and scores line by line.
            scores.emplace_back(name, score);
        }
        file.close();
    }
}

// Saves the current leaderboard state back to the file.
void Leaderboard::save() {
    ofstream file(filename, ios::trunc); // Overwrite the file.
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.first << " " << entry.second << "\n"; // Write each score.
        }
        file.close();
    }
}

// Adds or updates a player's score, ensuring the leaderboard is sorted.
void Leaderboard::addResult(const string& playerName, int score) {
    // Check if the player already exists in the leaderboard.
    auto it = find_if(scores.begin(), scores.end(), [&](const auto& entry) {
        return entry.first == playerName;
    });

    if (it != scores.end()) {
        // Update the player's score if found.
        it->second = score;
    } else {
        // Add a new entry if the player is not found.
        scores.emplace_back(playerName, score);
    }

    // Sort the scores in descending order of scores (highest first).
    sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return b.second > a.second;
    });
    save(); // Save the updated leaderboard.
}

// Displays the leaderboard in a formatted way.
void Leaderboard::display() const {
    cout << "\n--- Leaderboard ---\n";
    for (const auto& entry : scores) {
        cout << entry.first << ": " << entry.second << "\n"; // Print each player's score.
    }
    cout << "-------------------\n";
}