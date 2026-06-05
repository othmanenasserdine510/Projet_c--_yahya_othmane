#ifndef UNDERGRADUATESTUDENT_H
#define UNDERGRADUATESTUDENT_H

#include "Student.h"
#include <iostream>
#include <iomanip>

// Étudiant en Licence
class UndergraduateStudent : public Student {
private:
    string major; // Filière (ex: "Informatique")

public:
    UndergraduateStudent(const string& name, int id, float gpa, const string& major)
        : Student(name, id, gpa), major(major) {}

    string getMajor() const { return major; }
    void   setMajor(const string& m) { major = m; }

    string getType() const override { return "Licence"; }

    // Bourse : 500 EUR si GPA >= 3.5, sinon 0
    double computeScholarship() const override {
        return (gpa >= 3.5f) ? 500.0 : 0.0;
    }

    void display() const override {
        cout << left
             << "[Licence]  ID: " << setw(5) << id
             << "| Nom: "         << setw(20) << name
             << "| GPA: "         << fixed << setprecision(1) << gpa
             << " | Filiere: "    << setw(20) << major
             << "| Bourse: "      << computeScholarship() << " EUR"
             << endl;
    }
};

#endif // UNDERGRADUATESTUDENT_H
