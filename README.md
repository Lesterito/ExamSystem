# 🎓 System Egzaminacyjny (ExamSystem)

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)]()
[![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)]()

Krótki opis: Konsolowa aplikacja w języku C++ służąca do przeprowadzania, zarządzania i oceniania egzaminów. Projekt stworzony w celu praktycznego zastosowania zaawansowanych koncepcji programowania obiektowego (OOP).

## 🚀 Główne funkcjonalności

System posiada podział na role użytkowników i obsługuje różne poziomy dostępu:

* **👨‍🎓 Moduł Studenta:** Rozwiązywanie przypisanych egzaminów, podgląd własnych wyników.
* **👨‍🏫 Moduł Wykładowcy (Lecturer):** Tworzenie zestawów pytań (`Questions`), przypisywanie egzaminów, generowanie raportów (`Report`).
* **⚙️ Moduł Administratora (Admin):** Zarządzanie kontami użytkowników i uprawnieniami w systemie.
* **💾 System zapisu:** Trwałe przechowywanie danych użytkowników i wyników za pomocą plików (np. `users.dat`).

## 🛠️ Technologie i Umiejętności

W tym projekcie wykorzystałem i utrwaliłem następujące koncepcje:
* C++ (Standard [np. C++17/C++20])
* Programowanie Obiektowe (Klasy, Dziedziczenie, Polimorfizm - np. klasy bazowe `User`)
* Zarządzanie pamięcią i wskaźniki
* Obsługa wyjątków (klasa `ExamException`)
* Operacje na plikach (I/O)

## 💻 Jak uruchomić projekt lokalnie

Projekt został skonfigurowany pod środowisko Microsoft Visual Studio.

1. Sklonuj repozytorium:
   ```bash
   git clone [https://github.com/](https://github.com/)[TWÓJ_NICK]/ExamSystem.git
