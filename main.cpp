#include "ExamSystem.h"
#include "ExamException.h"
#include <iostream>
#include <Windows.h>
#include <locale.h> 

int main() {
    // Ustawienie polskiego kodowania znaków dla konsoli.
    setlocale(LC_ALL, "Polish");
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    try {
        ExamSystem system;
        system.run();
    }
    catch (const ExamException& e) {
        // Obs³uga niestandardowych b³êdów krytycznych (np. b³¹d pliku)
        std::cerr << "\n--- B£¥D KRYTYCZNY ---\n";
        std::cerr << "Nie mo¿na uruchomiæ programu: " << e.what() << "\n";
        std::cerr << "Aplikacja zostanie zamkniêta.\n";
        return 1;
    }
    catch (const std::exception& e) {
        // Obs³uga innych b³êdów standardowych (np. z³a alokacja pamiêci)
        std::cerr << "\n--- B£¥D KRYTYCZNY (NIEOKREŒLONY) ---\n";
        std::cerr << "Wyst¹pi³ nieznany b³¹d standardowy: " << e.what() << "\n";
        return 2;
    }
    catch (...) {
        // Obs³uga wszystkich innych b³êdów
        std::cerr << "\n--- B£¥D KRYTYCZNY (FATALNY) ---\n";
        std::cerr << "Wyst¹pi³ nieznany b³¹d, który nie jest wyj¹tkiem C++.\n";
        return 3;
    }
    return 0; // Poprawne zakoñczenie programu
}