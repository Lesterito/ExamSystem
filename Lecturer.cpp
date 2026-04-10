#include "Lecturer.h"
#include "ExamSystem.h" 
#include "Exam.h"       
#include "Questions.h"  
#include "Utils.h"     
#include <iostream>     
#include <algorithm>    

// Wyœwietla menu opcji dla wyk³adowcy.
void Lecturer::displayMenu(ExamSystem& system) {
	Utils::clearScreen();
	std::cout << "\n--- MENU WYK£ADOWCY (" << username << ") --- \n";
	std::cout << "Twoje przedmioty: ";
	for (const auto& s : subjects) { std::cout << s << " "; }
	std::cout << "\n";

	std::cout << "1. Utwórz egzamin\n";
	std::cout << "2. Edytuj egzamin\n";
	std::cout << "3. Przegl¹daj raporty\n";
	std::cout << "0. Wyloguj siê\n";

	int choice = Utils::getValidatedInt(0, 3);

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
	case 0:
		std::cout << "Wylogowywanie...\n";
		break;
	default:
		std::cout << "Nieprawid³owa opcja. Spróbuj ponownie.\n";
	}
}

// Prowadzi przez proces tworzenia nowego egzaminu.
void Lecturer::createExam(ExamSystem& system) {
	std::cout << "\n--- Tworzenie nowego egzaminu ---\n";

	if (subjects.empty()) {
		std::cout << "Nie masz przypisanych ¿adnych przedmiotów. Nie mo¿esz utworzyæ egzaminu.\n";
		return;
	}

	std::string selectedSubject;

	if (subjects.size() == 1) {
		selectedSubject = subjects[0];
		std::cout << "Wybrano jedyny dostêpny przedmiot: " << selectedSubject << "\n";
	}
	else {
		std::cout << "Dostêpne przedmioty:\n";
		for (size_t i = 0; i < subjects.size(); ++i) {
			std::cout << i + 1 << ". " << subjects[i] << "\n";
		}
		int choice = Utils::getValidatedInt(1, static_cast<int>(subjects.size()));
		selectedSubject = subjects[choice - 1];
	}

	// Zbieranie danych o egzaminie
	std::cout << "Podaj tytu³ egzaminu: ";
	std::string title = Utils::readLine(std::cin);

	std::cout << "Podaj limit czasu (w minutach): ";
	int timeLimit = Utils::getValidatedInt(1, 300);

	std::cout << "Podaj dozwolon¹ liczbê prób (np. 1): ";
	int maxAttempts = Utils::getValidatedInt(1, 10);

	// Tworzenie obiektu egzaminu
	int newExamID = system.getNextExamID();
	auto newExam = std::make_unique<Exam>(newExamID, title, timeLimit, selectedSubject, maxAttempts);

	Exam* examPtr = newExam.get();

	system.addExam(std::move(newExam));
	std::cout << "Egzamin '" << examPtr->getTitle() << "' zosta³ zapisany w systemie (bez pytañ).\n";

	char addMore;
	std::cout << "Czy chcesz dodaæ pierwsze pytanie teraz? (T/N): ";
	std::cin >> addMore;
	Utils::clearInputBuffer();

	bool questionsAdded = false;
	while (std::toupper(addMore) == 'T') {
		questionsAdded = true;
		// Wywo³ujemy funkcjê pomocnicz¹ odziedziczon¹ z 'Staff'
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

// Logika wyboru egzaminu do edycji (tylko z w³asnych przedmiotów).
void Lecturer::editExam(ExamSystem& system) {
	std::cout << "\n--- Edycja Egzaminu ---\n";
	std::map<int, Exam*> editableExams;

	for (const auto& pair : system.getAllExams()) {
		Exam* exam = pair.second.get();
		if (canAccessSubject(exam->getSubject())) {
			editableExams[exam->getID()] = exam;
		}
	}

	if (editableExams.empty()) {
		std::cout << "Nie masz ¿adnych egzaminów do edycji.\n";
		return;
	}

	std::cout << "Egzaminy, które mo¿esz edytowaæ:\n";
	for (const auto& pair : editableExams) {
		Exam* exam = pair.second;
		std::cout << "ID: " << exam->getID() << " | Tytu³: " << exam->getTitle() << " | Przedmiot: " << exam->getSubject() << "\n";
	}

	std::cout << "Podaj ID egzaminu do edycji: ";
	int examID = Utils::getValidatedInt(0, 99999);

	if (examID == 0) {
		std::cout << "Anulowano edycjê egzaminu.\n";
		return;
	}

	auto it = editableExams.find(examID);
	if (it == editableExams.end()) {
		std::cout << "Nie masz dostêpu do egzaminu o podanym ID lub egzamin nie istnieje.\n";
		return;
	}
	else {
		examEditMenu(it->second, system);
	}
}

// Logika wyœwietlania raportów (tylko z w³asnych przedmiotów).
void Lecturer::viewReports(ExamSystem& system) {
	std::cout << "\n--- Przegl¹danie Raportów (Wyk³adowca) ---\n";

	std::map<int, Exam*> accessibleExams;
	for (const auto& pair : system.getAllExams()) {
		Exam* exam = pair.second.get();
		if (canAccessSubject(exam->getSubject())) {
			accessibleExams[exam->getID()] = exam;
		}
	}

	if (accessibleExams.empty()) {
		std::cout << "Nie masz ¿adnych egzaminów, dla których mo¿esz zobaczyæ raporty.\n";
		return;
	}

	std::cout << "Egzaminy, dla których mo¿esz generowaæ raporty:\n";
	for (const auto& pair : accessibleExams) {
		Exam* exam = pair.second;
		std::cout << "  ID: " << exam->getID()
			<< " | Tytu³: " << exam->getTitle() << "\n";
	}

	std::cout << "Podaj ID egzaminu (0 aby anulowaæ):\n";
	int examID = Utils::getValidatedInt(0, 99999);
	if (examID == 0) return;

	if (accessibleExams.find(examID) == accessibleExams.end()) {
		std::cout << "B³¹d: Nie masz uprawnieñ do tego egzaminu lub ID jest nieprawid³owe.\n";
	}
	else {
		std::unique_ptr<ReportData> report = system.generateReport(examID);
		if (report) {
			Utils::printReport(*report);
		}
	}
}

// Sprawdza, czy przedmiot jest na liœcie 'subjects' wyk³adowcy.
bool Lecturer::canAccessSubject(const std::string& subject) const {
	return std::find(subjects.begin(), subjects.end(), subject) != subjects.end();
}

// Zapisuje dane specyficzne dla wyk³adowcy (listê przedmiotów).
void Lecturer::saveToFile(std::ostream& out) const {
	User::saveToFile(out);

	out << subjects.size() << "\n";
	for (const auto& subject : subjects) {
		out << subject << "\n";
	}
}

// Wczytuje dane specyficzne dla wyk³adowcy.
void Lecturer::loadFromFile(std::istream& in) {
	int subjectCount;
	in >> subjectCount;
	subjects.clear();

	for (int i = 0; i < subjectCount; ++i) {
		std::string subject;
		std::getline(in >> std::ws, subject);
		subjects.push_back(subject);
	}
}