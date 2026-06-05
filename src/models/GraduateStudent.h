#ifndef GRADUATESTUDENT_H
#define GRADUATESTUDENT_H

#include "Student.h"
#include <iostream>
#include <iomanip>

// Étudiant en Master
class GraduateStudent : public Student {
private:
    string researchTopic; // Sujet de recherche

public:
    GraduateStudent(const string& name, int id, float gpa, const string& researchTopic)
        : Student(name, id, gpa), researchTopic(researchTopic) {}

    string getResearchTopic() const { return researchTopic; }
    void   setResearchTopic(const string& t) { researchTopic = t; }

    string getType() const override { return "Master"; }

    // Bourse : GPA * 300 EUR
    double computeScholarship() const override {
        return gpa * 300.0;
    }

    void display() const override {
        cout << left
             << "[Master]   ID: " << setw(5) << id
             << "| Nom: "         << setw(20) << name
             << "| GPA: "         << fixed << setprecision(1) << gpa
             << " | Sujet: "      << setw(25) << researchTopic
             << "| Bourse: "      << computeScholarship() << " EUR"
             << endl;
    }
};

#endif // GRADUATESTUDENT_H
