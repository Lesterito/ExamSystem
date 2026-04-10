#pragma once
#include "Staff.h"
#include <vector>
#include <string>


class Lecturer : public Staff {
private:
	// Lista przedmiotów, do których ten wyk³adowca ma uprawnienia.
	std::vector<std::string> subjects;

public:
	Lecturer(int id, const std::string& uname, const std::string& passHash, const std::vector<std::string>& subs = {}) : Staff(id, uname, passHash), subjects(subs) {}

	void displayMenu(ExamSystem& system) override;
	void createExam(ExamSystem& system) override;
	void editExam(ExamSystem& system) override;
	void viewReports(ExamSystem& system) override;
	void saveToFile(std::ostream& out) const override;
	void loadFromFile(std::istream& in) override;
	std::string getUserType() const override { return "LECTURER"; }
	bool canAccessSubject(const std::string& subject) const;
};