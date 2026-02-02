#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

struct Movie {
    string title;
    string genre;
    string mood;
    string length;
};

vector<Movie> loadMovies() {
    vector<Movie> movies;
    ifstream file("../data/movies.csv");

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

    return movies;
}

int main() {
    vector<Movie> movies = loadMovies();

    if (movies.empty()) {
        cout << "No movies loaded.\n";
        return 0;
    }

    cout << "\n===== MoyaMoya Movie Decision Engine =====\n\n";

    string mood, length, energy, alone;

    cout << "Mood (Happy/Calm/Intense): ";
    cin >> mood;

    cout << "Time (Short/Medium/Long): ";
    cin >> length;

    cout << "Energy (Low/Medium/High): ";
    cin >> energy;

    cout << "Watching alone? (Yes/No): ";
    cin >> alone;

    map<string,int> score;

    for (auto &m : movies) {
        int s = 0;

        if (m.mood == mood) s += 3;
        if (m.length == length) s += 2;

        if (energy == "High" && m.genre == "Action") s += 2;
        if (energy == "Low" && m.genre == "Drama") s += 2;

        if (alone == "Yes" && m.genre == "Romance") s += 1;
        if (alone == "No" && m.genre == "Comedy") s += 2;

        score[m.title] = s;
    }

    vector<pair<int,string>> ranked;

    for (auto &x : score)
        ranked.push_back({x.second, x.first});

    sort(ranked.rbegin(), ranked.rend());

    cout << "\nTop Picks:\n";

    for (int i = 0; i < min(3, (int)ranked.size()); i++) {
        cout << i+1 << ". " << ranked[i].second
             << " (score " << ranked[i].first << ")\n";
    }

    int confidence = 60;
    if (ranked.size() >= 2)
        confidence = min(90, 60 + (ranked[0].first - ranked[1].first)*10);

    cout << "\nConfidence: " << confidence << "%\n";

    cout << "\nWhy this?\n";
    cout << "- matched your mood\n";
    cout << "- fits your time\n";
    cout << "- adjusted for energy\n";

    cout << "\n========================================\n";

    return 0;
}