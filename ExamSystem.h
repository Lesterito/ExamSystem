#pragma once

#include "User.h"
#include "Exam.h"
#include "Utils.h"
#include "Report.h"
#include <map>
#include <memory>
#include <string>

// Deklaracje wyprzedzaj¹ce dla konkretnych typów u¿ytkowników
class Student;
class Lecturer;
class Admin;

// G³ówna klasa zarz¹dzaj¹ca ca³ym systemem.
class ExamSystem {
public:
	ExamSystem();
	~ExamSystem();

	void run();

	int getNextExamID() { return nextExamID++; }
	int getNextQuestionID() { return nextQuestionID++; }

	void addExam(std::unique_ptr<Exam> exam) {
		exams[exam->getID()] = std::move(exam);
		forceSaveData();
	}

	void listAvailableExams() const;
	const std::map<int, std::unique_ptr<Exam>>& getAllExams() const { return exams; }
	Exam* getExamByID(int examID) const;
	std::unique_ptr<ReportData> generateReport(int ExamID);
	void listAllUsers() const;
	void createNewLecturer();
	void deleteUser();
	bool deleteExamByID(int examID);
	void forceSaveData() const;

private:
	std::map<std::string, std::unique_ptr<User>> users;
	std::map<int, std::unique_ptr<Exam>> exams;

	User* currentUser;
	bool isRunning;

	int nextUserID;
	int nextExamID;
	int nextQuestionID;

	void displayMainMenu();
	void login();
	void registerUser();

	void loadData();
	void saveData();
	void loadUsers();
	void saveUsers() const;
	void loadExams();
	void saveExams() const;

};