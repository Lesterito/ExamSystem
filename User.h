#pragma once
#include <string>
#include <iostream>

// Deklaracja wyprzedzaj¹ca, aby unikn¹æ do³¹czania "ExamSystem.h"
class ExamSystem;

//Abstrakcyjna klasa bazowa dla wszystkich u¿ytkowników systemu.
class User {
protected:
    std::string username;

    std::string passwordHash;

    int userID = 0;

public:
    User(int id, const std::string& uname, const std::string& passHash)
        : userID(id), username(uname), passwordHash(passHash) {
    }

    virtual ~User() {}

    // Czysta funkcja wirtualna do wyœwietlania menu specyficznego dla danego typu u¿ytkownika.   
    virtual void displayMenu(ExamSystem& system) = 0;

    /**
     * Czysta funkcja wirtualna zwracaj¹ca typ u¿ytkownika jako string.
     * U¿ywana przy zapisie do pliku.
     */
    virtual std::string getUserType() const = 0;

    std::string getUsername() const { return username; }

    int getUserID() const { return userID; }

    // Sprawdza, czy podane has³o pasuje do zapisanego.
    bool checkPassword(const std::string& pass) const { return passwordHash == pass; }

//   Zapisuje podstawowe dane u¿ytkownika (ID, login, has³o) do strumienia wyjœciowego.
    virtual void saveToFile(std::ostream& out) const {
        out << userID << "\n";
        out << username << "\n";
        out << passwordHash << "\n";
    }

    // Czysta funkcja wirtualna do wczytywania danych *specyficznych* dla klasy pochodnej.
    virtual void loadFromFile(std::istream& in) = 0;
};