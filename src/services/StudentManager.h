#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <cctype>
#include "../models/Student.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../exceptions/StudentExceptions.hpp"

using namespace std;

// Service principal gérant la collection d'étudiants (CRUD + tri + stats)
class StudentManager {
private:
    map<int, shared_ptr<Student>> students; // Stockage : ID -> Étudiant

public:
    // --------------------------------------------------
    // CRUD
    // --------------------------------------------------

    // Ajouter un étudiant ; lance DuplicateIDException si l'ID existe déjà
    void add(shared_ptr<Student> s) {
        if (students.count(s->getId()))
            throw DuplicateIDException(s->getId());
        students[s->getId()] = s;
    }

    // Supprimer un étudiant par ID ; lance StudentNotFoundException si absent
    void remove(int id) {
        if (!students.count(id))
            throw StudentNotFoundException(id);
        students.erase(id);
    }

    // Mettre à jour un étudiant existant
    void update(int id, shared_ptr<Student> updated) {
        if (!students.count(id))
            throw StudentNotFoundException(id);
        students[id] = updated;
    }

    // Trouver par ID ; lance StudentNotFoundException si absent
    shared_ptr<Student> findById(int id) const {
        auto it = students.find(id);
        if (it == students.end())
            throw StudentNotFoundException(id);
        return it->second;
    }

    // Recherche partielle par nom (insensible à la casse) — utilise std::copy_if
    vector<shared_ptr<Student>> findByName(const string& name) const {
        vector<shared_ptr<Student>> result;
        string lowerQuery = name;
        transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        // Extraire les valeurs du map dans un vecteur temporaire
        vector<shared_ptr<Student>> all;
        for (const auto& [id, s] : students)
            all.push_back(s);

        // Utilisation de std::copy_if (STL) pour filtrer
        copy_if(all.begin(), all.end(), back_inserter(result),
            [&lowerQuery](const shared_ptr<Student>& s) {
                string lowerName = s->getName();
                transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
                return lowerName.find(lowerQuery) != string::npos;
            });
        return result;
    }

    // --------------------------------------------------
    // Tri STL
    // --------------------------------------------------

    // Tri décroissant par GPA
    vector<shared_ptr<Student>> sortByGpa() const {
        vector<shared_ptr<Student>> result;
        for (const auto& [id, s] : students)
            result.push_back(s);

        sort(result.begin(), result.end(),
            [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
                return a->getGpa() > b->getGpa();
            });
        return result;
    }

    // Tri alphabétique insensible à la casse
    vector<shared_ptr<Student>> sortByName() const {
        vector<shared_ptr<Student>> result;
        for (const auto& [id, s] : students)
            result.push_back(s);

        sort(result.begin(), result.end(),
            [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
                string na = a->getName(), nb = b->getName();
                transform(na.begin(), na.end(), na.begin(), ::tolower);
                transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
                return na < nb;
            });
        return result;
    }

    // --------------------------------------------------
    // Statistiques
    // --------------------------------------------------

    double averageGpa() const {
        if (students.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& [id, s] : students)
            sum += s->getGpa();
        return sum / students.size();
    }

    // Comptage par type (std::count_if)
    int countByType(const string& type) const {
        return count_if(students.begin(), students.end(),
            [&type](const pair<int, shared_ptr<Student>>& p) {
                return p.second->getType() == type;
            });
    }

    int count() const { return (int)students.size(); }

    const map<int, shared_ptr<Student>>& getAll() const { return students; }

    void clear() { students.clear(); }
};

#endif // STUDENTMANAGER_H
