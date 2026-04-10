#pragma once
#include <string>
#include <vector>
#include <numeric>  
#include <algorithm>   

// Struktura danych do przechowywania wygenerowanego raportu.
struct ReportData {
    int examID = 0;
    std::string examTitle;
    int attempts = 0;
    double averageScore = 0.0;
    double minScore = 0.0;
    double maxScore = 0.0;

private:
    std::vector<double> allScores;

public:
    void addScore(double score) {
        allScores.push_back(score);
    }

    // Oblicza statystyki (œrednia, min, max) na podstawie zebranych wyników.
    void calculateStats() {
        if (allScores.empty()) {
            attempts = 0;
            return;
        }

        attempts = static_cast<int>(allScores.size());

        averageScore = std::accumulate(allScores.begin(), allScores.end(), 0.0) / attempts;
        minScore = *std::min_element(allScores.begin(), allScores.end());
        maxScore = *std::max_element(allScores.begin(), allScores.end());
    }
};