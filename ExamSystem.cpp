#include "ExamSystem.h"
#include "Exam.h"      
#include "Student.h"    
#include "Lecturer.h"  
#include "Admin.h"     
#include "Questions.h" 
#include "ExamException.h"

#include <iostream>
#include <fstream>
#include <limits>  
#include <iomanip>  

// Globalne sta³e dla nazw plików
const std::string USERS_FILE = "users.dat";
const std::string EXAMS_FILE = "exams.dat";

ExamSystem::ExamSystem()
	: currentUser(nullptr), isRunning(true), nextUserID(1), nextExamID(1), nextQuestionID(1) {
	try {
		loadData();
	}
	catch (const ExamException& e) {
		std::cerr << "B£¥D KRYTYCZNY PODCZAS STARTU: " << e.what() << "\n";
		std::cerr << "Program zostanie zamkniêty.\n";
		isRunning = false;
	}
}

// Destruktor: Zapisuje dane przy wyjœciu (jeœli start siê powiód³).
ExamSystem::~ExamSystem() {
	std::cout << "Zamykanie Systemu Egzaminacyjnego... \n";
	if (isRunning) {
		forceSaveData();
	}
}

// G³ówna pêtla programu, obs³uguje prze³¹czanie miêdzy menu.
void ExamSystem::run() {
	while (isRunning) {
		if (currentUser == nullptr) {
			Utils::clearScreen();
			displayMainMenu();
			int choice = Utils::getValidatedInt(0, 2);
			switch (choice) {
			case 1: login(); break;
			case 2: registerUser(); break;
			case 0: isRunning = false; break;
			}
		}
		else {
			currentUser->displayMenu(*this);
			currentUser = nullptr; // Wylogowanie po powrocie z menu
		}
	}
	std::cout << "Do widzenia!\n";
}

// Wyœwietla menu g³ówne.
void ExamSystem::displayMainMenu() {
	std::cout << "\n--- WITAJ W SYSTEMIE EGZAMINACYJNYM ---\n";
	std::cout << "1. Zaloguj siê\n";
	std::cout << "2. Zarejestruj siê\n";
	std::cout << "0. WyjdŸ\n";
}

// Logika logowania u¿ytkownika.
void ExamSystem::login() {
	std::string username, password;
	std::cout << "Podaj login: ";
	std::cin >> username;
	std::cout << "Podaj has³o: ";
	std::cin >> password;

	auto it = users.find(username);

	if (it == users.end()) {
		std::cout << "B³¹d: U¿ytkownik o tej nazwie nie istnieje.\n";
		return;
	}

	User* userToLogin = it->second.get();

	// Sprawdzenie has³a
	if (userToLogin->checkPassword(password)) {
		currentUser = userToLogin;
		std::cout << "Zalogowano pomyœlnie jako " << currentUser->getUsername() << " (" << currentUser->getUserType() << ")\n";
	}
	else {
		std::cout << "B³¹d: Nieprawid³owe has³o.\n";
	}
}

// Logika rejestracji nowego studenta.
void ExamSystem::registerUser() {
	std::string username;
	std::cout << "Rejestracja nowego studenta.\n";
	std::cout << "Podaj login: ";
	std::cin >> username;

	// Sprawdzenie, czy login jest wolny
	if (users.count(username)) {
		std::cout << "B³¹d: U¿ytkownik o tej nazwie ju¿ istnieje.\n";
		return;
	}

	std::string password;
	while (true) {
		std::cout << "Podaj has³o (min. 6 znaków): ";
		password = Utils::readLine(std::cin);

		if (password.length() < 6) {
			std::cout << "B³¹d: Has³o jest za krótkie. Spróbuj ponownie.\n";
		}
		else {
			break;
		}
	}

	int newID = nextUserID++;
	auto newStudent = std::make_unique<Student>(newID, username, password);
	users[username] = std::move(newStudent);
	std::cout << "Rejestracja pomyœlna. Mo¿esz siê teraz zalogowaæ.\n";

	forceSaveData();
	Utils::pause();
}

// G³ówna funkcja wczytuj¹ca dane i ustawiaj¹ca liczniki ID.
void ExamSystem::loadData() {
	loadUsers();
	loadExams();

	int maxUserId = 0;
	for (const auto& pair : users) {
		if (pair.second->getUserID() > maxUserId) {
			maxUserId = pair.second->getUserID();
		}
	}
	nextUserID = maxUserId + 1;

	int maxExamId = 0;
	for (const auto& pair : exams) {
		if (pair.first > maxExamId) {
			maxExamId = pair.first;
		}
	}
	nextExamID = maxExamId + 1;
}

void ExamSystem::saveData() {
	saveUsers();
	saveExams();
}

// Wczytuje u¿ytkowników z pliku users.dat.
void ExamSystem::loadUsers() {
	std::ifstream file(USERS_FILE);

	if (!file.is_open()) {
		// Przypadek pierwszego uruchomienia
		std::cout << "Plik 'users.dat' nie znaleziony. Tworzenie domyœlnego administratora...\n";
		auto admin = std::make_unique<Admin>(nextUserID++, "admin", "admin"); // ID 1
		users["admin"] = std::move(admin);
		return;
	}

	int userCount;
	file >> userCount;

	if (file.fail()) {
		// Przypadek uszkodzonego/pustego pliku
		std::cout << "Plik 'users.dat' jest pusty lub uszkodzony. Start z domyœlnym adminem. \n";
		auto admin = std::make_unique<Admin>(nextUserID++, "admin", "admin"); // ID 1
		users["admin"] = std::move(admin);
		file.clear();
		file.close();
		return;
	}

	// Pêtla wczytuj¹ca u¿ytkowników
	for (int i = 0; i < userCount; i++) {
		std::string userType;
		int userID;
		std::string username, passwordHash;

		file >> userType >> userID >> username >> passwordHash;
		if (file.fail()) {
			std::cerr << "B³¹d odczytu danych u¿ytkownika nr " << i << ". Plik mo¿e byæ uszkodzony.\n";
			break;
		}

		std::unique_ptr<User> newUser = nullptr;
		if (userType == "STUDENT") {
			newUser = std::make_unique<Student>(userID, username, passwordHash);
		}
		else if (userType == "LECTURER") {
			newUser = std::make_unique<Lecturer>(userID, username, passwordHash);
		}
		else if (userType == "ADMIN") {
			newUser = std::make_unique<Admin>(userID, username, passwordHash);
		}
		else {
			std::cerr << "Nieznany typ u¿ytkownika w pliku: " << userType << "\n";
			continue;
		}

		if (newUser) {
			newUser->loadFromFile(file); // Wczytanie danych specyficznych (np. wyników)
			users[newUser->getUsername()] = std::move(newUser);
		}
	}
	file.close();
}

// Zapisuje wszystkich u¿ytkowników do pliku.
void ExamSystem::saveUsers() const {
	std::ofstream file(USERS_FILE);
	file << users.size() << "\n";

	for (const auto& pair : users) {
		const auto& user = pair.second;
		file << user->getUserType() << "\n";
		user->saveToFile(file);
		file << "\n";
	}
	std::cout << "Zapisano " << users.size() << " u¿ytkowników.\n";
}

// Wczytuje egzaminy z pliku exams.dat.
void ExamSystem::loadExams() {
	std::ifstream file(EXAMS_FILE);
	if (!file.is_open()) {
		std::cout << "Plik 'exams.dat' nie znaleziony. Start z pust¹ baz¹ egzaminów.\n";
		return;
	}

	int examCount;
	file >> examCount;

	if (file.fail()) {
		std::cout << "Baza egzaminów jest pusta.\n";
		file.clear();
		file.close();
		return;
	}

	// Pêtla wczytuj¹ca egzaminy
	for (int i = 0; i < examCount; ++i) {
		auto newExam = std::make_unique<Exam>(0, "", 0, "", 1);
		newExam->loadFromFile(file);

		if (file.fail()) {
			std::cerr << "B³¹d odczytu danych egzaminu nr " << i << ". Plik mo¿e byæ uszkodzony.\n";
			break;
		}

		int loadedExamID = newExam->getID();
		exams[loadedExamID] = std::move(newExam); 
	}

	if (!exams.empty()) {
		std::cout << "Wczytano " << exams.size() << " egzaminów.\n";
	}
	file.close(); 
}

// Zapisuje wszystkie egzaminy do pliku.
void ExamSystem::saveExams() const {
	std::ofstream file(EXAMS_FILE);

	if (!file.is_open()) {
		throw FileIOException("Nie mo¿na otworzyæ pliku 'exams.dat' do zapisu. Brak uprawnieñ lub plik jest zablokowany.");
	}

	file << exams.size() << "\n";
	for (const auto& pair : exams) {
		pair.second->saveToFile(file);
		file << "\n";

		if (file.fail()) {
			file.close();
			throw FileIOException("Wyst¹pi³ b³¹d podczas zapisu egzaminu ID: " + std::to_string(pair.first));
		}
	}
	file.close();
	std::cout << "Zapisano " << exams.size() << " egzaminów.\n";
}

// Wyœwietla listê wszystkich dostêpnych egzaminów.
void ExamSystem::listAvailableExams() const {
	std::cout << "\n--- DOSTÊPNE EGZAMINY ---\n";
	if (exams.empty()) {
		std::cout << "Brak dostêpnych egzaminów.\n";
		return;
	}
	for (const auto& pair : exams) {
		const Exam* exam = pair.second.get();
		std::cout << "ID: " << exam->getID() << " | Tytu³: " << exam->getTitle() << " | Przedmiot: " << exam->getSubject() << "\n";
	}
}

Exam* ExamSystem::getExamByID(int examID) const {
	auto it = exams.find(examID);
	if (it != exams.end()) {
		return it->second.get();
	}
	return nullptr; 
}

// Generuje raport statystyczny dla danego egzaminu.
std::unique_ptr<ReportData> ExamSystem::generateReport(int examID) {
	Exam* exam = getExamByID(examID);
	if (exam == nullptr) {
		return nullptr;
	}

	auto report = std::make_unique<ReportData>();
	report->examID = examID;
	report->examTitle = exam->getTitle();

	for (const auto& pair : users) {
		User* user = pair.second.get();

		Student* student = dynamic_cast<Student*>(user);
		if (student != nullptr) {
			double score = student->getScore(examID);
			if (score != -1.0) { // -1.0 oznacza "nie pisa³"
				report->addScore(score);
			}
		}
	}
	report->calculateStats();
	return report;
}

// Wyœwietla listê wszystkich u¿ytkowników w systemie.
void ExamSystem::listAllUsers() const {
	std::cout << "\n--- Lista U¿ytkowników Systemu ---\n";
	std::cout << "ID   | Login            | Typ\n";
	std::cout << "--------------------------------------\n";
	for (const auto& pair : users) {
		User* user = pair.second.get();
		std::cout << std::setw(4) << user->getUserID() << " | "
			<< std::setw(16) << std::left << user->getUsername() << " | "
			<< std::setw(10) << user->getUserType() << std::right << "\n";
	}
	std::cout << std::setfill(' ');
}

// Logika tworzenia nowego wyk³adowcy.
void ExamSystem::createNewLecturer() {
	std::cout << "\n--- Tworzenie Nowego Wyk³adowcy ---\n";
	std::cout << "Podaj login: ";
	std::string username = Utils::readLine(std::cin);
	if (users.count(username)) {
		std::cout << "B³¹d: U¿ytkownik o tej nazwie ju¿ istnieje.\n";
		return;
	}

	std::string password;
	while (true) {
		std::cout << "Podaj has³o (min. 6 znaków): ";
		password = Utils::readLine(std::cin);
		if (password.length() < 6) {
			std::cout << "B³¹d: Has³o jest za krótkie. Spróbuj ponownie.\n";
		}
		else {
			break;
		}
	}

	// Pêtla do wprowadzania przedmiotów
	std::vector<std::string> subjects;
	std::string subject;
	std::cout << "Podaj przedmioty (wpisz '0' aby zakoñczyæ):\n";
	while (true) {
		std::cout << "Przedmiot: ";
		subject = Utils::readLine(std::cin);
		if (subject == "0") break;
		subjects.push_back(subject);
	}

	int newID = nextUserID++;
	auto lecturer = std::make_unique<Lecturer>(newID, username, password, subjects);

	users[username] = std::move(lecturer);
	std::cout << "Utworzono Wyk³adowcê: " << username << "\n";

	forceSaveData(); 
}

// Logika usuwania u¿ytkownika.
void ExamSystem::deleteUser() {
	std::cout << "\n--- Usuwanie U¿ytkownika ---\n";
	listAllUsers();
	std::cout << "Podaj login u¿ytkownika do usuniêcia (lub '0' aby anulowaæ): ";
	std::string username = Utils::readLine(std::cin);

	if (username == "0") return;

	auto it = users.find(username);
	if (it == users.end()) {
		throw ValidationException("U¿ytkownik o tej nazwie nie istnieje.");
	}


	if (it->second->getUserID() == 1) {
		throw OperationException("Nie mo¿esz usun¹æ domyœlnego administratora (ID 1)!");
	}

	users.erase(it);
	std::cout << "U¿ytkownik '" << username << "' zosta³ pomyœlnie usuniêty.\n";

	forceSaveData();
}

// Wymusza zapis obu plików (users.dat i exams.dat).
void ExamSystem::forceSaveData() const {
	std::cout << "\n[System] Rozpoczynanie zapisu danych...\n";
	try {
		saveUsers();
		saveExams();
		std::cout << "[System] Dane zosta³y pomyœlnie zapisane.\n";
	}
	catch (const ExamException& e) {
		std::cerr << "B£¥D KRYTYCZNY PODCZAS ZAPISU: " << e.what() << "\n";
	}
	catch (...) {
		std::cerr << "NIESPODZIEWANY B£¥D PODCZAS PRÓBY ZAPISU DANYCH. \n";
	}
}

bool ExamSystem::deleteExamByID(int examID) {
	auto it = exams.find(examID);
	if (it == exams.end()) {
		std::cout << "B³¹d: Egzamin o podanym ID nie istnieje.\n";
		return false;
	}

	std::string deletedTitle = it->second->getTitle();

	exams.erase(it);

	for (auto& userPair : users) {
		Student* student = dynamic_cast<Student*>(userPair.second.get());
		if (student != nullptr) {
			student->removeExamRecord(examID);
		}
	}

	std::cout << "Pomyœlnie usuniêto egzamin '" << deletedTitle << "' oraz wszystkie powi¹zane z nim wyniki.\n";

	forceSaveData();
	return true;
}