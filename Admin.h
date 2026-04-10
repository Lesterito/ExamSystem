#pragma once
#include "Staff.h"
class ExamSystem;

class Admin : public Staff {
private:
	void manageUsers(ExamSystem& system);
	void deleteExam(ExamSystem& system);
public:
	Admin(int id, const std::string& uname, const std::string& passHash) : Staff(id, uname, passHash) {}

	void displayMenu(ExamSystem& system) override;
	void createExam(ExamSystem& system) override;
	void editExam(ExamSystem& system) override;
	void viewReports(ExamSystem& system) override;
	void saveToFile(std::ostream& out) const override;
	void loadFromFile(std::istream& in) override;
	std::string getUserType() const override { return "ADMIN"; }
};