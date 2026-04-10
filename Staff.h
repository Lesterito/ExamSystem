#pragma once
#include "User.h"
#include <memory> // Dla std::shared_ptr

// Deklaracje wyprzedzaj¹ce
class Question;
class ExamSystem;
class Exam;

class Staff : public User {
protected:
    // Funkcja pomocnicza do tworzenia nowego pytania.
    std::shared_ptr<Question> createQuestionHelper(ExamSystem& system);

    // Funkcja pomocnicza wyœwietlaj¹ca menu edycji dla wybranego egzaminu.
    void examEditMenu(Exam* exam, ExamSystem& system);

public:
    Staff(int id, const std::string& uname, const std::string& passHash) : User(id, uname, passHash) {}
    virtual ~Staff() {}

    void displayMenu(ExamSystem& system) override;

    // --- Czyste funkcje wirtualne dla personelu ---

    virtual void createExam(ExamSystem& system) = 0;
    virtual void editExam(ExamSystem& system) = 0;
    virtual void viewReports(ExamSystem& system) = 0;

    void saveToFile(std::ostream& out) const override = 0;
    void loadFromFile(std::istream& in) override = 0;
};