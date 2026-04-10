#include "Admin.h"
#include "ExamSystem.h" 
#include "Exam.h"       
#include "ExamException.h" 
#include "Utils.h"     
#include <iostream>    

void Admin::displayMenu(ExamSystem& system) {
	Utils::clearScreen();
	std::cout << "\n--- MENU ADMINA (" << username << ") ---\n";
	std::cout << "1. Utwórz egzamin\n";
	std::cout << "2. Edytuj egzamin\n";
	std::cout << "3. Przegl¹daj raporty\n";
	std::cout << "4. Zarz¹dzaj u¿ytkownikami\n";
	std::cout << "5. Usuñ Egzamin\n";
	std::cout << "0. Wyloguj siê\n";

	int choice = Utils::getValidatedInt(0, 5);

	switch (choice) {
	case 1:
		createExam(system);
		Utils::pause();
		break;
	case 2:
		editExam(system);
		Utils::pause();
		break;
	case 3:
		viewReports(system);
		Utils::pause();
		break;
	case 4:
		manageUsers(system);
		break;
	case 5:
		deleteExam(system);
		Utils::pause();
		break;
	case 0:
		std::cout << "Wylogowano.\n";
		break;
	default:
		std::cout << "Nieprawid³owa opcja. Spróbuj ponownie.\n";
	}
}

// Prowadzi przez proces tworzenia nowego egzaminu (bez ograniczeñ).
void Admin::createExam(ExamSystem& system) {
	std::cout << "\n--- Tworzenie nowego egzaminu ---\n";

	std::cout << "Podaj tytu³ egzaminu: ";
	std::string title = Utils::readLine(std::cin);

	std::cout << "Podaj nazwê przedmiotu (np. Programowanie Obiektowe): ";
	std::string subject = Utils::readLine(std::cin);

	std::cout << "Podaj limit czasu (w minutach): ";
	int timeLimit = Utils::getValidatedInt(1, 300);

	std::cout << "Podaj dozwolon¹ liczbê prób (np. 1): ";
	int maxAttempts = Utils::getValidatedInt(1, 10);

	// Pobranie ID i utworzenie obiektu
	int newExamID = system.getNextExamID();
	auto newExam = std::make_unique<Exam>(newExamID, title, timeLimit, subject, maxAttempts);
	Exam* examPtr = newExam.get();
	system.addExam(std::move(newExam));
	std::cout << "Egzamin '" << examPtr->getTitle() << "' zosta³ zapisany w systemie.\n";

	// Pêtla dodawania pytañ
	char addMore;
	std::cout << "Czy chcesz dodaæ pierwsze pytanie teraz? (T/N): ";
	std::cin >> addMore;
	Utils::clearInputBuffer();
	bool questionsAdded = false;
	while (std::toupper(addMore) == 'T') {
		bool questionsAdded = true;
		std::shared_ptr<Question> question = createQuestionHelper(system);
		examPtr->addQuestion(question);
		std::cout << "Pytanie zosta³o dodane.\n";
		std::cout << "Czy chcesz dodaæ kolejne pytanie? (T/N): ";
		std::cin >> addMore;
		Utils::clearInputBuffer();
	}

	if (questionsAdded) {
		std::cout << "Aktualizowanie egzaminu o nowe pytania...\n";
		system.forceSaveData(); // Zapisz zmiany (dodane pytania)
		std::cout << "Egzamin zosta³ zaktualizowany.\n";
	}
}

// Logika wyboru egzaminu do edycji (bez ograniczeñ).
void Admin::editExam(ExamSystem& system) {
	std::cout << "\n--- Edytowanie egzaminu ---\n";
	if (system.getAllExams().empty()) {
		std::cout << "Brak egzaminów do edycji.\n";
		return;
	}

	system.listAvailableExams();

	std::cout << "Podaj ID egzaminu do edycji (0 aby anulowaæ): ";
	int examID = Utils::getValidatedInt(0, 99999);
	if (examID == 0) {
		std::cout << "Anulowano edycjê egzaminu.\n";
		return;
	}

	Exam* exam = system.getExamByID(examID);

	if (exam == nullptr) {
		std::cout << "Nie znaleziono egzaminu o podanym ID.\n";
	}
	else {
		examEditMenu(exam, system);
	}
}

// Logika wyœwietlania raportów (bez ograniczeñ).
void Admin::viewReports(ExamSystem& system) {
	std::cout << "\n--- Przegl¹danie Raportów ---\n";
	if (system.getAllExams().empty()) {
		std::cout << "Brak egzaminów w systemie.\n";
		return;
	}

	system.listAvailableExams();

	std::cout << "Podaj ID egzaminu, dla którego chcesz wygenerowaæ raport (0 aby anulowaæ): ";
	int examID = Utils::getValidatedInt(0, 99999);
	if (examID == 0) return;

	std::unique_ptr<ReportData> report = system.generateReport(examID);

	if (report == nullptr) {
		std::cout << "B³¹d. Egzamin o tym ID nie istnieje. \n";
	}
	else {
		Utils::printReport(*report);
	}
}

void Admin::manageUsers(ExamSystem& system) {
	bool managing = true;
	while (managing) {
		Utils::clearScreen();
		std::cout << "\n--- Zarz¹dzanie U¿ytkownikami ---\n";
		std::cout << "1. Wylistuj wszystkich u¿ytkowników\n";
		std::cout << "2. Utwórz nowego Wyk³adowcê\n";
		std::cout << "3. Usuñ u¿ytkownika\n";
		std::cout << "0. Powrót do menu g³ównego\n";

		int choice = Utils::getValidatedInt(0, 3);

		switch (choice) {
		case 1:
			system.listAllUsers();
			Utils::pause();
			break;
		case 2:
			system.createNewLecturer();
			Utils::pause();
			break;
		case 3: {
			try {
				system.deleteUser();
			}
			catch (const ValidationException& e) {
				std::cout << "B³¹d: " << e.what() << "\n";
			}
			catch (const OperationException& e) {
				std::cout << "B³¹d: " << e.what() << "\n";
			}
			catch (const ExamException& e) {
				std::cout << "Niespodziewany b³¹d: " << e.what() << "\n";
			}
			Utils::pause();
			break;
		}
		case 0:
			managing = false;
			break;
		}
	}
}

void Admin::saveToFile(std::ostream& out) const {
	User::saveToFile(out);
}

void Admin::loadFromFile(std::istream& in) {
}

void Admin::deleteExam(ExamSystem& system) {
	std::cout << "\n--- Usuwanie Egzaminu ---\n";
	if (system.getAllExams().empty()) {
		std::cout << "Brak egzaminów w systemie do usuniêcia.\n";
		return;
	}

	system.listAvailableExams();

	std::cout << "Podaj ID egzaminu, który chcesz trwale usun¹æ (0 aby anulowaæ): ";
	int examID = Utils::getValidatedInt(0, 99999);
	if (examID == 0) {
		std::cout << "Anulowano usuwanie.\n";
		return;
	}

	system.deleteExamByID(examID);
}