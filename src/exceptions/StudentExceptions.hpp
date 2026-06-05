#ifndef STUDENT_EXCEPTIONS_H
#define STUDENT_EXCEPTIONS_H

#include <exception>
#include <string>

using namespace std;

// Exception lancée si l'ID est inférieur à 1000
class InvalidIDException : public exception {
    string msg;
public:
    explicit InvalidIDException(int id)
        : msg("ID invalide (doit etre >= 1000) : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Exception lancée si le GPA est hors de la plage [0.0, 4.0]
class InvalidGradeException : public exception {
    string msg;
public:
    explicit InvalidGradeException(float gpa)
        : msg("GPA hors plage [0.0 - 4.0] : " + to_string(gpa)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Exception lancée si l'étudiant recherché n'existe pas
class StudentNotFoundException : public exception {
    string msg;
public:
    explicit StudentNotFoundException(int id)
        : msg("Etudiant introuvable avec ID : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Exception lancée si un ID est déjà utilisé dans le gestionnaire
class DuplicateIDException : public exception {
    string msg;
public:
    explicit DuplicateIDException(int id)
        : msg("ID deja utilise : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

#endif // STUDENT_EXCEPTIONS_H
