#pragma once
#include "User.h"
#include <map>

// Przechowuje dane o podejœciach studenta do egzaminu.
struct ExamAttemptData {
	int attemptsTaken = 0;
	double bestScore = 0.0;
};
class Student : public User {
private:
	// Mapa przechowuj¹ca postêpy studenta w egzaminach (ID Egzaminu -> Dane o podejœciach).
	std::map<int, ExamAttemptData> examScores;

	void startExam(ExamSystem& system);
	void viewScores(const ExamSystem& system) const;

public:
	Student(int id, const std::string& uname, const std::string& passHash) : User(id, uname, passHash) {}

	void displayMenu(ExamSystem& system) override;
	std::string getUserType() const override { return "STUDENT"; }
	void saveToFile(std::ostream& out) const override;
	void loadFromFile(std::istream& in) override;

	// Zwraca najlepszy wynik studenta dla danego egzaminu.
	double getScore(int examID) const;
	void removeExamRecord(int examID);
};