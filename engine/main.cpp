#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

struct Movie {
    string title;
    string type;      // Movie / Series
    string language;  // English / Korean / Chinese / Thai / Hindi
    string genre;
    string mood;
    string length;
};

struct Result {
    int score;
    string title;
    vector<string> reasons;

    // For max heap
    bool operator<(const Result& other) const {
        return score < other.score;
    }
};

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
        getline(ss, m.type, ',');
        getline(ss, m.language, ',');
        getline(ss, m.genre, ',');
        getline(ss, m.mood, ',');
        getline(ss, m.length, ',');

        movies.push_back(m);
    }

    return movies;
}

int main() {
    auto movies = loadMovies();

    if (movies.empty()) {
        cout << "No data loaded.\n";
        return 0;
    }

    cout << "\n===== MoyaMoya Movie / Series Decision Engine =====\n\n";

    string contentType, language, mood, length, energy, alone;

    cout << "Movie or Series? (Movie/Series): ";
    cin >> contentType;

    cout << "Language (English/Korean/Chinese/Thai/Hindi): ";
    cin >> language;

    cout << "Mood (Happy/Calm/Intense): ";
    cin >> mood;

    cout << "Time (Short/Medium/Long): ";
    cin >> length;

    cout << "Energy (Low/Medium/High): ";
    cin >> energy;

    cout << "Watching alone? (Yes/No): ";
    cin >> alone;

    priority_queue<Result> pq;

    for (auto &m : movies) {

        // Hard filters
        if (m.type != contentType) continue;
        if (m.language != language) continue;

        Result r;
        r.score = 0;
        r.title = m.title;

        // Base bonuses
        r.score += 2;
        r.reasons.push_back("matched your content type");

        r.score += 2;
        r.reasons.push_back("matched your preferred language");

        if (m.mood == mood) {
            r.score += 3;
            r.reasons.push_back("matched your mood");
        }

        if (m.length == length) {
            r.score += 2;
            r.reasons.push_back("fits your time");
        }

        if (energy == "High" && m.genre == "Action") {
            r.score += 2;
            r.reasons.push_back("high energy action bonus");
        }

        if (energy == "Low" && m.genre == "Drama") {
            r.score += 2;
            r.reasons.push_back("low energy drama bonus");
        }

        if (alone == "No" && m.genre == "Comedy") {
            r.score += 2;
            r.reasons.push_back("group comedy bonus");
        }

        pq.push(r);
    }

    cout << "\nTop Picks:\n";

    vector<Result> top;

    for (int i = 0; i < 3 && !pq.empty(); i++) {
        auto r = pq.top(); pq.pop();
        top.push_back(r);

        cout << i + 1 << ". " << r.title << " (score " << r.score << ")\n";
        for (auto &reason : r.reasons)
            cout << "   - " << reason << "\n";
    }

    if (top.size() >= 2) {
        int diff = top[0].score - top[1].score;
        int confidence = min(90, 60 + diff * 10);
        cout << "\nConfidence: " << confidence << "%\n";
    }

    cout << "\n===============================================\n";

    return 0;
}