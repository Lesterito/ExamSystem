#include "Questions.h"
#include <iostream> 
#include <cctype>  

// Konstruktor pytania wielokrotnego wyboru.
MultipleChoiceQuestion::MultipleChoiceQuestion(const std::string& text, const std::vector<std::string>& opts, int correctIdx)
	:options(opts), correctOptionIndex(correctIdx) {
	questionText = text;
}

void MultipleChoiceQuestion::display() const {
	std::cout << questionText << "\n";
	char optionChar = 'A';
	for (const auto& option : options) {
		std::cout << optionChar++ << ". " << option << "\n";
	}
}

bool MultipleChoiceQuestion::checkAnswer(const std::string& answer) const {
	if (answer.empty()) return false;
	char selectedOption = std::toupper(answer[0]);
	return (selectedOption - 'A') == correctOptionIndex;
}

std::string MultipleChoiceQuestion::getCorrectAnswer() const {
	return options[correctOptionIndex];
}

// Zapisuje pytanie do pliku.
void MultipleChoiceQuestion::saveToFile(std::ostream& out) const {
	out << questionID << "\n";
	out << questionText << "\n";

	out << options.size() << "\n";
	for (const auto& option : options) {
		out << option << "\n";
	}
	out << correctOptionIndex << "\n";
}

// Wczytuje pytanie z pliku.
void MultipleChoiceQuestion::loadFromFile(std::istream& in) {
	in >> questionID;
	std::getline(in >> std::ws, questionText);

	int optionsCount;
	in >> optionsCount;
	options.clear();

	for (int i = 0; i < optionsCount; i++) {
		std::string option;
		std::getline(in >> std::ws, option);
		options.push_back(option);
	}
	in >> correctOptionIndex;
}