#include "Utils.h"
#include <iomanip>
#include <stdexcept>
#include <cstdlib> 

namespace Utils
{
    // Czyœci bufor wejœciowy std::cin po b³êdnej operacji.
    void clearInputBuffer() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Wczytuje ca³¹ liniê tekstu (np. tytu³ egzaminu).
    std::string readLine(std::istream& in) {
        std::string line;
        std::getline(in >> std::ws, line);
        return line;
    }

    // Wczytuje od u¿ytkownika liczbê i waliduje j¹.
    int getValidatedInt(int min, int max) {
        std::string input;
        int choice = 0;

        while (true) {
            std::cout << "Twój wybór: ";
            input = Utils::readLine(std::cin);

            try {
                choice = std::stoi(input);

                // SprawdŸ zakres
                if (choice < min || choice > max) {
                    std::cout << "Wybór poza zakresem (" << min << "-" << max << "). Spróbuj ponownie.\n";
                }
                else {
                    return choice;
                }
            }
            catch (const std::invalid_argument&) {
                // B³¹d, jeœli input to np. "abc"
                std::cout << "Niepoprawny format. WprowadŸ liczbê.\n";
            }
            catch (const std::out_of_range&) {
                // B³¹d, jeœli liczba jest za du¿a dla typu int
                std::cout << "Wprowadzona liczba jest zbyt du¿a. Spróbuj ponownie.\n";
            }
        }
    }

    // Wyœwietla sformatowany raport statystyczny.
    void printReport(const ReportData& report) {
        std::cout << "\n--- RAPORT EGZAMINU ---\n";
        std::cout << "Tytu³: " << report.examTitle << "(ID: " << report.examID << ")\n";

        if (report.attempts == 0) {
            std::cout << "Brak podejœæ do egzaminu.\n";
            std::cout << "-----------------------\n";
            return;
        }
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Liczba podejœæ: " << report.attempts << "\n";
        std::cout << "Œredni wynik: " << report.averageScore << "%\n";
        std::cout << "Najni¿szy wynik: " << report.minScore << "%\n";
        std::cout << "Najwy¿szy wynik: " << report.maxScore << "%\n";
        std::cout << "---------------------------\n";

        std::cout.unsetf(std::ios_base::floatfield);
        std::cout << std::setprecision(6);
    }
    void clearScreen() {
        system("cls");
    }
    void pause() {
        std::cout << "\nNaciœnij Enter, aby kontynuowaæ...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }
}
