#include "Exam.h"
#include "Questions.h"  
#include "Utils.h"      
#include <iostream>     
#include <chrono>      
#include <algorithm>    
#include <random>       

Exam::Exam(int id, const std::string& t, int time, const std::string& sub, int maxAttempts)
	: examID(id), title(t), timeLimitMinutes(time), subject(sub), maxAttemptsAllowed(maxAttempts) {
}

std::string Exam::getSubject() const {
	return subject;
}
std::string Exam::getTitle() const {
	return title;
}
int Exam::getTimeLimit() const {
	return timeLimitMinutes;
}
int Exam::getMaxAttempts() const {
	return maxAttemptsAllowed;
}

void Exam::setTitle(const std::string& t) {
	title = t;
}
void Exam::setTimeLimit(int time) {
	timeLimitMinutes = time;
}
void Exam::setMaxAttempts(int maxAttempts) {
	maxAttemptsAllowed = maxAttempts;
}

void Exam::addQuestion(const std::shared_ptr<Question>& question) {
	questions.push_back(question);
}

// G≥Ûwna funkcja uruchamiajπca egzamin dla studenta.
double Exam::conductExam() {
	if (questions.empty()) {
		std::cout << "\n=============================================\n";
		std::cout << "B£•D: Ten egzamin nie zawiera jeszcze øadnych pytaÒ.\n";
		std::cout << "Zg≥oú to administratorowi lub wyk≥adowcy.\n";
		std::cout << "Naciúnij Enter, aby wrÛciÊ do menu...\n";
		std::cout << "=============================================\n";
		Utils::readLine(std::cin);
		return 0.0;
	}

	std::cout << "\n=============================================\n";
	std::cout << "Rozpoczynanie egzaminu: " << title << "\n";
	std::cout << "Masz " << timeLimitMinutes << " minut.\n";
	std::cout << "=============================================\n\n";

	// Losowanie kolejnoúci pytaÒ (WymÛg projektu)
	auto rng = std::default_random_engine(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::shuffle(questions.begin(), questions.end(), rng);

	auto startTime = std::chrono::steady_clock::now();
	// Uøywam sekund zamiast minut do celÛw testowych
	auto endTime = startTime + std::chrono::seconds(timeLimitMinutes);

	int score = 0;
	int questionNumber = 1;

	for (const auto& q_ptr : questions) {
		// Sprawdzenie, czy czas siÍ nie skoÒczy≥
		if (std::chrono::steady_clock::now() > endTime) {
			std::cout << "\nCzas egzaminu minπ≥!\n";
			std::cout << "Pozosta≥e pytania nie zostanπ zaliczone.\n";
			break;
		}

		std::cout << "\n--- Pytanie " << questionNumber++ << "/" << questions.size() << " --- \n";
		q_ptr->display();

		std::string userAnswer;
		std::cout << "Twoja odpowiedü: ";
		std::cin >> userAnswer;
		Utils::clearInputBuffer();

		if (q_ptr->checkAnswer(userAnswer)) {
			std::cout << " -> Poprawna odpowiedü!\n";
			score++;
		}
		else {
			std::cout << " -> B≥Ídna odpowiedü. Poprawna to: " << q_ptr->getCorrectAnswer() << "\n";
		}
	}

	std::cout << "\n=============================================\n";
	std::cout << "Egzamin zakoÒczony!\n";

	double finalScore = 0.0;
	if (!questions.empty()) { 
		finalScore = (static_cast<double>(score) / questions.size()) * 100.0;
	}
	std::cout << "TwÛj wynik: " << score << "/" << questions.size()
		<< " (" << finalScore << "%)\n";
	std::cout << "=============================================\n";

	return finalScore;
}

// Zapisuje ca≥y egzamin (z pytaniami) do pliku.
void Exam::saveToFile(std::ostream& out) const {
	out << examID << "\n";
	out << title << "\n"; 
	out << timeLimitMinutes << "\n";
	out << subject << "\n";
	out << maxAttemptsAllowed << "\n";

	out << questions.size() << "\n";
	for (const auto& q_ptr : questions) {
		out << q_ptr->getQuestionType() << "\n"; 
		q_ptr->saveToFile(out); 
	}
}

// Wczytuje ca≥y egzamin (z pytaniami) z pliku.
void Exam::loadFromFile(std::istream& in) {
	in >> examID;
	std::getline(in >> std::ws, title);
	in >> timeLimitMinutes;
	std::getline(in >> std::ws, subject);

	maxAttemptsAllowed = 1;
	if (!in.eof()) {
		in >> maxAttemptsAllowed;
	}
	if (in.fail()) {
		in.clear(); 
		maxAttemptsAllowed = 1;
	}

	int questionCount;
	in >> questionCount;
	questions.clear();

	for (int i = 0; i < questionCount; i++) {
		std::string questionType;
		in >> questionType;

		std::shared_ptr<Question> newQuestion = nullptr;

		if (questionType == "MCQ") {
			newQuestion = std::make_shared<MultipleChoiceQuestion>("", std::vector<std::string>{}, 0);
		}

		if (newQuestion) {
			newQuestion->loadFromFile(in);
			questions.push_back(newQuestion);
		}
		else {
			std::cerr << "Nieznany typ pytania: " << questionType << " podczas wczytywania\n";
		}
	}
}

// Wyúwietla listÍ pytaÒ (dla menu edycji).
void Exam::listQuestionsForEdit() const {
	std::cout << "\n--- Pytania w tym egzaminie ---\n";
	if (questions.empty()) {
		std::cout << "Brak pytaÒ w tym egzaminie.\n";
		return;
	}

	for (size_t i = 0; i < questions.size(); i++) {
		std::string text = questions[i]->getText();
		if (text.length() > 50) {
			text = text.substr(0, 47) + "...";
		}
		std::cout << "Nr: " << (i + 1) << " | " << text << "\n";
	}
}

bool Exam::deleteQuestionByIndex(size_t index) {
	if (index < 1 || index > questions.size()) {
		return false;
	}
	questions.erase(questions.begin() + (index - 1));
	std::cout << "Pytanie " << index << " zosta≥o usuniÍte.\n";
	return true;
}