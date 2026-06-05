#ifndef PHDSTUDENT_H
#define PHDSTUDENT_H

#include "Student.h"
#include <iostream>
#include <iomanip>

// Doctorant
class PhDStudent : public Student {
private:
    string supervisor;  // Directeur de thèse
    int    yearOfStudy; // Année de doctorat (1, 2, 3...)

public:
    PhDStudent(const string& name, int id, float gpa,
               const string& supervisor, int yearOfStudy)
        : Student(name, id, gpa), supervisor(supervisor), yearOfStudy(yearOfStudy) {}

    string getSupervisor()  const { return supervisor;  }
    int    getYearOfStudy() const { return yearOfStudy; }
    void   setSupervisor(const string& s)  { supervisor  = s; }
    void   setYearOfStudy(int y)           { yearOfStudy = y; }

    string getType() const override { return "Doctorat"; }

    // Bourse fixe : 1200 EUR/mois
    double computeScholarship() const override { return 1200.0; }

    void display() const override {
        cout << left
             << "[Doctorat] ID: " << setw(5) << id
             << "| Nom: "         << setw(20) << name
             << "| GPA: "         << fixed << setprecision(1) << gpa
             << " | Directeur: "  << setw(15) << supervisor
             << "| Annee: "       << yearOfStudy
             << " | Bourse: "     << computeScholarship() << " EUR"
             << endl;
    }
};

#endif // PHDSTUDENT_H
