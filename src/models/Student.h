#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "../exceptions/StudentExceptions.hpp"

using namespace std;

// Classe abstraite représentant un étudiant générique
class Student {
protected:
    string name;  // Nom complet
    int    id;    // Identifiant unique (>= 1000)
    float  gpa;   // Note entre 0.0 et 4.0

public:
    Student(const string& name, int id, float gpa) {
        if (id < 1000)
            throw InvalidIDException(id);
        if (gpa < 0.0f || gpa > 4.0f)
            throw InvalidGradeException(gpa);
        if (name.empty())
            throw invalid_argument("Le nom ne peut pas etre vide.");
        this->name = name;
        this->id   = id;
        this->gpa  = gpa;
    }

    virtual ~Student() {}

    // Méthodes virtuelles pures — chaque sous-classe DOIT les redéfinir
    virtual void   display()              const = 0;
    virtual string getType()              const = 0;
    virtual double computeScholarship()   const = 0;

    // Accesseurs
    string getName() const { return name; }
    int    getId()   const { return id;   }
    float  getGpa()  const { return gpa;  }

    // Mutateurs avec validation
    void setName(const string& n) {
        if (n.empty()) throw invalid_argument("Le nom ne peut pas etre vide.");
        name = n;
    }

    void setGpa(float g) {
        if (g < 0.0f || g > 4.0f)
            throw InvalidGradeException(g);
        gpa = g;
    }
};

#endif // STUDENT_H
