#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>cd
#include <map>
#include <algorithm>
using namespace std;

struct Movie {
    string title;
    string genre;
    string mood;
    string length;
};

// Load movies from CSV
vector<Movie> loadMovies() {
    vector<Movie> movies;
    ifstream file("../data/movies.csv");

    if (!file.is_open()) {
        cout << "Could not open movies.csv\n";
        return movies;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Movie m;

        getline(ss, m.title, ',');
        getline(ss, m.genre, ',');
        getline(ss, m.mood, ',');
        getline(ss, m.length, ',');

        movies.push_back(m);
    }

    file.close();
    return movies;
}

int main() {
    vector<Movie> movies = loadMovies();

    if (movies.empty()) {
        cout << "No movies loaded.\n";
        return 0;
    }

    cout << "===== ClearChoice : Movie Decision Engine =====\n\n";

    string mood, length;

    cout << "How are you feeling? (Happy/Calm/Intense): ";
    cin >> mood;

    cout << "How much time do you have? (Short/Medium/Long): ";
    cin >> length;

    // Score for each movie
    map<string, int> score;

    // Simple weighted scoring
    for (auto &m : movies) {
        score[m.title] = 0;

        if (m.mood == mood) score[m.title] += 3;
        if (m.length == length) score[m.title] += 2;
    }

    // Rank movies
    vector<pair<int,string>> ranked;
    for (auto &s : score) {
        ranked.push_back({s.second, s.first});
    }

    sort(ranked.rbegin(), ranked.rend()); // highest score first

    cout << "\nTop Recommendations:\n";

    int k = min(3, (int)ranked.size());
    for (int i = 0; i < k; i++) {
        cout << i + 1 << ". " << ranked[i].second
             << " (score: " << ranked[i].first << ")\n";
    }

    // Confidence calculation (very simple)
    if (ranked.size() >= 2) {
        int diff = ranked[0].first - ranked[1].first;
        int confidence = min(90, 50 + diff * 10);
        cout << "\nConfidence: " << confidence << "%\n";
    } else {
        cout << "\nConfidence: 80%\n";
    }

    cout << "\nWhy this?\n";
    cout << "- Matches your mood\n";
    cout << "- Fits your available time\n";

    cout << "\n=============================================\n";

    return 0;
}