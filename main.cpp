#include <iostream>
#include <memory>
#include "src/models/Student.h"
#include "src/models/UndergraduateStudent.h"
#include "src/models/GraduateStudent.h"
#include "src/models/PhDStudent.h"
#include "src/services/StudentManager.h"
#include "src/persistence/PersistenceManager.h"
#include "src/exceptions/StudentExceptions.hpp"

using namespace std;

int main() {
    StudentManager mgr;
    string dataFile = "data/students.txt";

    cout << "=============================================" << endl;
    cout << "  SYSTEME DE GESTION DES ETUDIANTS" << endl;
    cout << "  Parties 3 & 4 — Exceptions + Persistance" << endl;
    cout << "=============================================" << endl << endl;

    // ============================================
    // PARTIE 4 : Chargement au démarrage
    // ============================================
    cout << "[PARTIE 4] Chargement des donnees depuis " << dataFile << " ..." << endl;
    PersistenceManager::load(mgr, dataFile);
    cout << "✓ Donnees chargees (" << mgr.count() << " etudiants)" << endl << endl;

    // ============================================
    // TEST 1 : InvalidIDException — ID < 1000
    // ============================================
    cout << "[TEST 1] InvalidIDException — ID invalide (42)" << endl;
    try {
        auto s = make_shared<UndergraduateStudent>("Test ID", 42, 3.0f, "Informatique");
        mgr.add(s);
    } catch (const InvalidIDException& e) {
        cerr << "  [ERREUR ID]  " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 2 : InvalidGradeException — GPA > 4.0
    // ============================================
    cout << "[TEST 2] InvalidGradeException — GPA trop eleve (5.5)" << endl;
    try {
        auto s = make_shared<UndergraduateStudent>("Test GPA+", 1001, 5.5f, "Informatique");
        mgr.add(s);
    } catch (const InvalidGradeException& e) {
        cerr << "  [ERREUR GPA] " << e.what() << endl;
    } catch (const InvalidIDException& e) {
        cerr << "  [ERREUR ID]  " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 3 : InvalidGradeException — GPA < 0.0
    // ============================================
    cout << "[TEST 3] InvalidGradeException — GPA negatif (-1.0)" << endl;
    try {
        auto s = make_shared<UndergraduateStudent>("Test GPA-", 1002, -1.0f, "Informatique");
        mgr.add(s);
    } catch (const InvalidGradeException& e) {
        cerr << "  [ERREUR GPA] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 4 : Création d'étudiants valides
    // ============================================
    cout << "[TEST 4] Creation d'etudiants VALIDES" << endl;
    try {
        auto ug = make_shared<UndergraduateStudent>("Amrani Kenza", 3001, 3.8f, "Informatique");
        mgr.add(ug);
        cout << "  ✓ Licence ajoute (ID: 3001)" << endl;
    } catch (const exception& e) {
        cerr << "  [ERREUR] " << e.what() << endl;
    }
    try {
        auto gr = make_shared<GraduateStudent>("Belhadj Youssef", 3002, 3.5f, "Vision par Ordinateur");
        mgr.add(gr);
        cout << "  ✓ Master ajoute  (ID: 3002)" << endl;
    } catch (const exception& e) {
        cerr << "  [ERREUR] " << e.what() << endl;
    }
    try {
        auto ph = make_shared<PhDStudent>("Chaoui Leila", 3003, 3.9f, "Pr. Mansouri", 2);
        mgr.add(ph);
        cout << "  ✓ Doctorat ajoute (ID: 3003)" << endl;
    } catch (const exception& e) {
        cerr << "  [ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 5 : DuplicateIDException
    // ============================================
    cout << "[TEST 5] DuplicateIDException — ID 3001 deja utilise" << endl;
    try {
        auto dup = make_shared<UndergraduateStudent>("Doublon", 3001, 3.0f, "Mathematiques");
        mgr.add(dup);
    } catch (const DuplicateIDException& e) {
        cerr << "  [DOUBLON ID] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 6 : StudentNotFoundException — findById
    // ============================================
    cout << "[TEST 6] StudentNotFoundException — findById(9999)" << endl;
    try {
        auto s = mgr.findById(9999);
    } catch (const StudentNotFoundException& e) {
        cerr << "  [INTROUVABLE] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 7 : StudentNotFoundException — remove
    // ============================================
    cout << "[TEST 7] StudentNotFoundException — remove(5555)" << endl;
    try {
        mgr.remove(5555);
    } catch (const StudentNotFoundException& e) {
        cerr << "  [INTROUVABLE] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 8 : findById valide
    // ============================================
    cout << "[TEST 8] findById valide — ID 3001" << endl;
    try {
        auto s = mgr.findById(3001);
        cout << "  ✓ Trouve : " << s->getName() << " (" << s->getType() << ")" << endl;
    } catch (const exception& e) {
        cerr << "  [ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 9 : setGpa() invalide
    // ============================================
    cout << "[TEST 9] InvalidGradeException — setGpa(6.0) sur etudiant existant" << endl;
    try {
        auto s = mgr.findById(3001);
        s->setGpa(6.0f);
    } catch (const InvalidGradeException& e) {
        cerr << "  [ERREUR GPA] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 10 : setGpa() valide
    // ============================================
    cout << "[TEST 10] setGpa() valide — modification a 3.9" << endl;
    try {
        auto s = mgr.findById(3001);
        s->setGpa(3.9f);
        cout << "  ✓ GPA mis a jour : " << s->getGpa() << endl;
    } catch (const exception& e) {
        cerr << "  [ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // Affichage tri par GPA
    // ============================================
    cout << "--- Liste triee par GPA (decroissant) ---" << endl;
    for (const auto& s : mgr.sortByGpa())
        s->display();
    cout << endl;

    // ============================================
    // Sauvegarde
    // ============================================
    cout << "=============================================" << endl;
    PersistenceManager::save(mgr, dataFile);
    cout << "✓ Donnees sauvegardees dans " << dataFile << endl;
    cout << "  Nombre d'etudiants : " << mgr.count() << endl;
    cout << "  Moyenne GPA        : " << mgr.averageGpa() << endl;
    cout << "=============================================" << endl;

    return 0;
}
