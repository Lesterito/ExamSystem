#pragma once
#include <string>
#include <vector>

class Question {
protected:
	std::string questionText;
	int questionID;
public:
	virtual ~Question() {}

	virtual void display() const = 0;
	virtual bool checkAnswer(const std::string& answer) const = 0;
	virtual std::string getCorrectAnswer() const = 0;
	virtual void saveToFile(std::ostream& out) const = 0;
	virtual void loadFromFile(std::istream& in) = 0;
	virtual std::string getQuestionType() const = 0;
	std::string getText() const { return questionText; }
};

// Klasa pochodna dla pytañ wielokrotnego wyboru.
class MultipleChoiceQuestion : public Question {
private:
	std::vector<std::string> options;
	int correctOptionIndex; 

public:
	MultipleChoiceQuestion(const std::string& text, const std::vector<std::string>& opts, int correctIdx);
	void display() const override;
	bool checkAnswer(const std::string& answer) const override;
	std::string getCorrectAnswer() const override;
	void saveToFile(std::ostream& out) const override;
	void loadFromFile(std::istream& in) override;
	std::string getQuestionType() const override { return "MCQ"; }
};