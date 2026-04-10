#include "Staff.h"
#include "ExamSystem.h"
#include "Questions.h" 
#include "Utils.h"      
#include "Exam.h"      

#include <iostream>
#include <string>
#include <vector>

void Staff::displayMenu(ExamSystem& system) {
    std::cout << "B³¹d: Wywo³ano domyœlne menu personelu.";
}

// Funkcja pomocnicza do tworzenia nowego pytania.
std::shared_ptr<Question> Staff::createQuestionHelper(ExamSystem& system) {
    std::cout << "\n-- Dodawanie nowego pytania --\n";
    std::cout << "Typ pytania: Wielokrotnego wyboru (MCQ)\n";

    std::cout << "Podaj treœæ pytania: ";
    std::string text = Utils::readLine(std::cin);

    std::vector<std::string> options;
    int optionCount = 0;
    std::cout << "Podaj liczbê opcji odpowiedzi (np. 4): ";
    optionCount = Utils::getValidatedInt(2, 6);

    for (int i = 0; i < optionCount; ++i) {
        std::cout << "Podaj opcjê " << static_cast<char>('A' + i) << ": ";
        options.push_back(Utils::readLine(std::cin));
    }

    char correctOptionChar;
    int correctIndex = -1;
    // Pêtla waliduj¹ca poprawn¹ odpowiedŸ (A, B, C...)
    while (correctIndex < 0 || correctIndex >= optionCount) {
        std::cout << "Podaj poprawn¹ odpowiedŸ (A-" << static_cast<char>('A' + optionCount - 1) << "): ";
        std::cin >> correctOptionChar;
        Utils::clearInputBuffer();
        correctIndex = std::toupper(correctOptionChar) - 'A';

        if (correctIndex < 0 || correctIndex >= optionCount) {
            std::cout << "Niepoprawna opcja. Spróbuj ponownie.\n";
        }
    }
    return std::make_shared<MultipleChoiceQuestion>(text, options, correctIndex);
}

// Pêtla pod-menu do edycji w³aœciwoœci egzaminu.
void Staff::examEditMenu(Exam* exam, ExamSystem& system) {
    bool editing = true;
    while (editing) {
        Utils::clearScreen();
        std::cout << "\n--- Edycja Egzaminu: " << exam->getTitle() << " ---\n";
        std::cout << "1. Zmieñ tytu³\n";
        std::cout << "2. Zmieñ limit czasu\n";
        std::cout << "3. Zmieñ liczbê prób\n";
        std::cout << "4. Dodaj nowe pytanie\n";
        std::cout << "5. Usuñ pytanie\n";
        std::cout << "0. Zakoñcz edycjê\n";

        int choice = Utils::getValidatedInt(0, 5);
        switch (choice) {
        case 1: {
            std::cout << "Podaj nowy tytu³: ";
            std::string newTitle = Utils::readLine(std::cin);
            exam->setTitle(newTitle);
            std::cout << "Tytu³ zaktualizowany.\n";
            break;
        }
        case 2: {
            std::cout << "Podaj nowy limit czasu (w minutach): ";
            int newTime = Utils::getValidatedInt(1, 300);
            exam->setTimeLimit(newTime);
            std::cout << "Limit czasu zaktualizowany.\n";
            break;
        }
        case 3: {
            std::cout << "Obecna liczba prób: " << exam->getMaxAttempts() << "\n";
            std::cout << "Podaj now¹ liczbê prób (1-10): ";
            int newAttempts = Utils::getValidatedInt(1, 10);
            exam->setMaxAttempts(newAttempts);
            std::cout << "Liczba prób zaktualizowana.\n";
            break;
        }
        case 4: {
            std::shared_ptr<Question> newQuestion = createQuestionHelper(system);
            exam->addQuestion(newQuestion);
            std::cout << "Nowe pytanie dodane.\n";
            break;
        }
        case 5: { 
            if (exam->getQuestions().empty()) {
                std::cout << "Brak pytañ do usuniêcia.\n";
                Utils::pause();
                break;
            }
            exam->listQuestionsForEdit();
            std::cout << "Podaj ID pytania do usuniêcia: ";
            int questionID = Utils::getValidatedInt(0, 99999);

            if (exam->deleteQuestionByIndex(questionID)) {
                std::cout << "Pytanie usuniête.\n";
            }
            else {
                std::cout << "Niepoprawne ID pytania.\n";
            }
            Utils::pause();
            break;
        }
        case 0:
            editing = false;
            system.forceSaveData(); // Zapiszujemy zmiany przy wyjœciu
            break;
        }
    }
    std::cout << "Zakoñczono edycjê egzaminu '" << exam->getTitle() << "'.\n";
}