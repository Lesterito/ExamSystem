#include "Student.h"
#include "ExamSystem.h" 
#include "Utils.h"     
#include <iostream>    

void Student::displayMenu(ExamSystem& system) {
	bool keepMenuOpen = true;
	while (keepMenuOpen) {
		Utils::clearScreen();
		std::cout << "\n--- MENU STUDENTA (" << username << ") ---\n";
		std::cout << "1. Rozpocznij egzamin\n";
		std::cout << "2. Zobacz moje wyniki\n";
		std::cout << "0. Wyloguj (powrót do menu g³ównego)\n";

		int choice = Utils::getValidatedInt(0, 2);

		switch (choice) {
		case 1:
			startExam(system);
			Utils::pause();
			break;
		case 2:
			viewScores(system);
			Utils::pause();
			break;
		case 0:
			std::cout << "Wylogowywanie...\n";
			keepMenuOpen = false;
			break;
		}
	}
}

void Student::loadFromFile(std::istream& in) {
	int scoreCount;
	in >> scoreCount;
	examScores.clear();

	for (int i = 0; i < scoreCount; i++) {
		int examID;
		ExamAttemptData data;
		in >> examID >> data.attemptsTaken >> data.bestScore;

		// Zabezpieczenie przed uszkodzonym plikiem
		if (in.fail()) {
			break;
		}
		examScores[examID] = data;
	}
}

void Student::saveToFile(std::ostream& out) const {
	User::saveToFile(out);

	// Zapisz dane specyficzne dla studenta
	out << examScores.size() << "\n";
	for (const auto& pair : examScores) {
		out << pair.first << " "
			<< pair.second.attemptsTaken << " "
			<< pair.second.bestScore << "\n";
	}
}

// Logika odpowiedzialna za proces rozpoczynania egzaminu.
void Student::startExam(ExamSystem& system) {
	std::cout << "\n--- Rozpocznij Egzamin ---\n";

	system.listAvailableExams();
	if (system.getAllExams().empty()) {
		return;
	}

	std::cout << "\nWybierz ID egzaminu, który chcesz rozpocz¹æ (0 aby anulowaæ): ";
	int examID = Utils::getValidatedInt(0, 1000000);
	if (examID == 0) {
		std::cout << "Anulowano rozpoczêcie egzaminu.\n";
		return;
	}

	Exam* examToTake = system.getExamByID(examID);
	if (examToTake == nullptr) {
		std::cout << "Nie znaleziono egzaminu o podanym ID.\n";
		return;
	}

	int allowedAttempts = examToTake->getMaxAttempts();

	// Pobierz (lub stwórz nowy) wpis o postêpach dla tego egzaminu
	ExamAttemptData& progress = examScores[examID];

	// SprawdŸ, czy student ma jeszcze dostêpne próby
	if (progress.attemptsTaken >= allowedAttempts) {
		std::cout << "B³¹d: Wykorzysta³eœ ju¿ wszystkie " << allowedAttempts
			<< " próby dla tego egzaminu.\n";
		std::cout << "Twój najlepszy zapisany wynik: " << progress.bestScore << "%\n";
		return;
	}

	std::cout << "To jest Twoja próba " << (progress.attemptsTaken + 1)
		<< " z " << allowedAttempts << " dozwolonych.\n";

	// Ostateczne potwierdzenie
	std::cout << "Czy na pewno chcesz rozpocz¹æ egzamin: " << examToTake->getTitle() << "? \n ";
	std::cout << "Bêdziesz mieæ " << examToTake->getTimeLimit() << " minut na jego ukoñczenie. Czas startuje TERAZ. \n";
	std::cout << "Wpisz 'TAK' aby potwierdziæ, lub cokolwiek innego aby anulowaæ: ";

	std::string confirm;
	std::cin >> confirm;
	Utils::clearInputBuffer();

	if (confirm != "TAK") {
		std::cout << "Anulowano rozpoczêcie egzaminu.\n";
		return;
	}

	// Uruchom egzamin
	double score = examToTake->conductExam();
	progress.attemptsTaken++;

	if (score > progress.bestScore) {
		progress.bestScore = score; // Zapisz nowy najlepszy wynik
		std::cout << "To Twój nowy najlepszy wynik dla tego egzaminu!\n";
	}
	else {
		std::cout << "Twój najlepszy wynik (" << progress.bestScore
			<< "%) nie zosta³ pobity.\n";
	}

	std::cout << "Pozosta³o prób: "
		<< (allowedAttempts - progress.attemptsTaken) << "\n";

	system.forceSaveData();
}

void Student::viewScores(const ExamSystem& system) const {
	std::cout << "\n--- Moje Wyniki ---\n";
	if (examScores.empty()) {
		std::cout << "Nie przyst¹pi³eœ jeszcze do ¿adnego egzaminu.\n";
		return;
	}

	const auto& exams = system.getAllExams();

	// PrzejdŸ przez wszystkie zapisane wyniki
	for (const auto& pair : examScores) {
		int examID = pair.first;
		double score = pair.second.bestScore;
		int attempts = pair.second.attemptsTaken;

		std::string title = "Nieznany Egzamin";
		int maxAttempts = 1;

		auto it = exams.find(examID);
		if (it != exams.end()) {
			title = it->second->getTitle();
			maxAttempts = it->second->getMaxAttempts();
		}
		std::cout << "Egzamin ID: " << examID << " | Tytu³: " << title << " | Wynik: " << score << "% | Próby: " << attempts << "/" << maxAttempts << "\n";
	}
}

// Zwraca najlepszy wynik studenta (u¿ywane przez system Raportów).
double Student::getScore(int examID) const {
	auto it = examScores.find(examID);
	if (it != examScores.end()) {
		return it->second.bestScore;
	}
	return -1.0; // Zwróci -1, jeœli student nie pisa³
}
void Student::removeExamRecord(int examID) {
	examScores.erase(examID);
}