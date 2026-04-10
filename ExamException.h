#pragma once
#include <stdexcept>
#include <string>

// Bazowa klasa wyj¹tku dla ca³ego systemu.
class ExamException : public std::runtime_error {
public:
    explicit ExamException(const std::string& message)
        : std::runtime_error(message) {}
};

// Wyj¹tek dla b³êdów operacji I/O (pliki).
class FileIOException : public ExamException {
public:
    explicit FileIOException(const std::string& message)
        : ExamException("B³¹d Pliku (I/O): " + message) {
    }
};

// Wyj¹tek dla b³êdów logicznych (np. próba usuniêcia admina).
class OperationException : public ExamException {
public:
    explicit OperationException(const std::string& message)
        :ExamException("B³¹d Operacji: " + message) {
    }
};

// Wyj¹tek dla b³êdów walidacji (np. z³y input).
class ValidationException : public ExamException {
public:
    explicit ValidationException(const std::string& message)
        : ExamException("B³¹d Walidacji: " + message) {
    }
};