#pragma once
#include "Questions.h"
#include <vector>
#include <string>
#include <memory>

class Exam {
private:
	int examID;
	std::string title;
	int timeLimitMinutes;
	std::string subject;
	int maxAttemptsAllowed;
	std::vector<std::shared_ptr<Question>> questions;

public:
	Exam(int id, const std::string& t, int time, const std::string& sub, int maxAttempts);

	std::string getSubject() const;
	std::string getTitle() const;
	int getTimeLimit() const;
	int getMaxAttempts() const;
	int getID() const { return examID; }
	const std::vector<std::shared_ptr<Question>>& getQuestions() const { return questions; }

	void setTitle(const std::string& t);
	void setTimeLimit(int time);
	void setMaxAttempts(int maxAttempts);

	void addQuestion(const std::shared_ptr<Question>& question);
	double conductExam();

	void listQuestionsForEdit() const;
	bool deleteQuestionByIndex(size_t index);

	void saveToFile(std::ostream& out) const;
	void loadFromFile(std::istream& in);
};