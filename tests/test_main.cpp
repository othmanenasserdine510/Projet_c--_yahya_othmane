// tests/test_main.cpp — Suite de tests complète (Partie 6)
// Couvre : types, CRUD, tri, exceptions, persistance

#include <iostream>
#include <cassert>
#include <memory>
#include <fstream>
#include <string>
#include "../src/models/UndergraduateStudent.h"
#include "../src/models/GraduateStudent.h"
#include "../src/models/PhDStudent.h"
#include "../src/services/StudentManager.h"
#include "../src/persistence/PersistenceManager.h"
#include "../src/exceptions/StudentExceptions.hpp"

using namespace std;

// ─── Compteur de résultats ───────────────────────────────────────────────────
int total = 0, passed = 0, failed = 0;

void check(bool condition, const string& testName) {
    total++;
    if (condition) {
        passed++;
        cout << "  [OK]   " << testName << endl;
    } else {
        failed++;
        cerr << "  [FAIL] " << testName << endl;
    }
}

// ─── Groupe 1 : Types d'étudiants ────────────────────────────────────────────
void testStudentTypes() {
    cout << "\n=== Groupe 1 : Types & polymorphisme ===" << endl;

    // Licence
    auto ug = make_shared<UndergraduateStudent>("Amrani Kenza", 1001, 3.8f, "Informatique");
    check(ug->getType() == "Licence",   "UndergraduateStudent::getType() == Licence");
    check(ug->computeScholarship() == 500.0, "Bourse Licence GPA 3.8 => 500 EUR");

    auto ug2 = make_shared<UndergraduateStudent>("Test Low", 1010, 3.0f, "Maths");
    check(ug2->computeScholarship() == 0.0, "Bourse Licence GPA 3.0 => 0 EUR");

    // Master
    auto gr = make_shared<GraduateStudent>("Belhadj Youssef", 1002, 3.5f, "IA");
    check(gr->getType() == "Master",        "GraduateStudent::getType() == Master");
    check(gr->computeScholarship() == 1050.0, "Bourse Master GPA 3.5 => 1050 EUR");

    // Doctorat
    auto ph = make_shared<PhDStudent>("Chaoui Leila", 1003, 3.9f, "Pr. Mansouri", 2);
    check(ph->getType() == "Doctorat",         "PhDStudent::getType() == Doctorat");
    check(ph->computeScholarship() == 1200.0,  "Bourse Doctorat => 1200 EUR (fixe)");

    // Accesseurs
    check(ug->getId()   == 1001,         "getId()  correct");
    check(ug->getName() == "Amrani Kenza","getName() correct");
    check(ug->getGpa()  == 3.8f,         "getGpa() correct");

    // GPA limite
    auto boundary_lo = make_shared<UndergraduateStudent>("Min", 1020, 0.0f, "X");
    auto boundary_hi = make_shared<UndergraduateStudent>("Max", 1021, 4.0f, "X");
    check(boundary_lo->getGpa() == 0.0f, "GPA limite basse 0.0 accepte");
    check(boundary_hi->getGpa() == 4.0f, "GPA limite haute 4.0 accepte");
}

// ─── Groupe 2 : CRUD ──────────────────────────────────────────────────────────
void testCRUD() {
    cout << "\n=== Groupe 2 : CRUD ===" << endl;
    StudentManager mgr;

    auto s1 = make_shared<UndergraduateStudent>("Amrani Kenza",   2001, 3.8f, "Informatique");
    auto s2 = make_shared<GraduateStudent>     ("Belhadj Youssef",2002, 3.5f, "IA");
    auto s3 = make_shared<PhDStudent>          ("Chaoui Leila",   2003, 3.9f, "Pr. Mansouri", 1);

    mgr.add(s1); mgr.add(s2); mgr.add(s3);
    check(mgr.count() == 3, "add() : 3 etudiants ajoutes");

    // findById
    auto found = mgr.findById(2002);
    check(found->getName() == "Belhadj Youssef", "findById(2002) retourne le bon etudiant");

    // findByName — recherche partielle (sous-chaîne du nom)
    auto results = mgr.findByName("chaoui");
    check(!results.empty() && results[0]->getName() == "Chaoui Leila",
          "findByName(\"chaoui\") retourne Chaoui Leila");

    // update
    auto updated = make_shared<UndergraduateStudent>("Amrani K. (MAJ)", 2001, 3.9f, "Informatique");
    mgr.update(2001, updated);
    check(mgr.findById(2001)->getName() == "Amrani K. (MAJ)", "update() modifie correctement");

    // remove
    mgr.remove(2003);
    check(mgr.count() == 2, "remove() : count passe a 2");

    // averageGpa
    double avg = mgr.averageGpa();
    check(avg > 0.0, "averageGpa() retourne une valeur positive");
}

// ─── Groupe 3 : Tris ─────────────────────────────────────────────────────────
void testSort() {
    cout << "\n=== Groupe 3 : Tri STL ===" << endl;
    StudentManager mgr;

    mgr.add(make_shared<UndergraduateStudent>("Charlie", 3001, 2.5f, "X"));
    mgr.add(make_shared<GraduateStudent>     ("Alice",   3002, 3.9f, "Y"));
    mgr.add(make_shared<PhDStudent>          ("Bob",     3003, 3.0f, "Pr.Z", 1));
    mgr.add(make_shared<UndergraduateStudent>("Eve",     3004, 1.8f, "W"));
    mgr.add(make_shared<GraduateStudent>     ("David",   3005, 4.0f, "V"));

    // Tri GPA décroissant
    auto byGpa = mgr.sortByGpa();
    check(byGpa[0]->getGpa() >= byGpa[1]->getGpa() &&
          byGpa[1]->getGpa() >= byGpa[2]->getGpa(),
          "sortByGpa() : ordre decroissant verifie");

    // Tri alphabétique
    auto byName = mgr.sortByName();
    string n0 = byName[0]->getName(), n1 = byName[1]->getName();
    string l0 = n0, l1 = n1;
    transform(l0.begin(), l0.end(), l0.begin(), ::tolower);
    transform(l1.begin(), l1.end(), l1.begin(), ::tolower);
    check(l0 <= l1, "sortByName() : ordre alphabetique verifie");

    // countByType
    check(mgr.countByType("Licence") == 2, "countByType(Licence) == 2");
    check(mgr.countByType("Master")  == 2, "countByType(Master)  == 2");
    check(mgr.countByType("Doctorat")== 1, "countByType(Doctorat) == 1");
}

// ─── Groupe 4 : Exceptions ───────────────────────────────────────────────────
void testExceptions() {
    cout << "\n=== Groupe 4 : Exceptions ===" << endl;
    StudentManager mgr;

    // InvalidIDException
    bool caught = false;
    try { make_shared<UndergraduateStudent>("X", 42, 3.0f, "Y"); }
    catch (const InvalidIDException&) { caught = true; }
    check(caught, "InvalidIDException : ID=42 < 1000");

    // InvalidGradeException (trop élevé)
    caught = false;
    try { make_shared<UndergraduateStudent>("X", 4001, 5.5f, "Y"); }
    catch (const InvalidGradeException&) { caught = true; }
    check(caught, "InvalidGradeException : GPA=5.5 > 4.0");

    // InvalidGradeException (négatif)
    caught = false;
    try { make_shared<UndergraduateStudent>("X", 4002, -1.0f, "Y"); }
    catch (const InvalidGradeException&) { caught = true; }
    check(caught, "InvalidGradeException : GPA=-1.0 < 0.0");

    // DuplicateIDException
    auto s = make_shared<UndergraduateStudent>("Real", 4010, 3.0f, "Info");
    mgr.add(s);
    caught = false;
    try {
        auto dup = make_shared<UndergraduateStudent>("Dup", 4010, 2.0f, "Info");
        mgr.add(dup);
    } catch (const DuplicateIDException&) { caught = true; }
    check(caught, "DuplicateIDException : ID=4010 deja utilise");

    // StudentNotFoundException — findById
    caught = false;
    try { mgr.findById(9999); }
    catch (const StudentNotFoundException&) { caught = true; }
    check(caught, "StudentNotFoundException : findById(9999)");

    // StudentNotFoundException — remove
    caught = false;
    try { mgr.remove(8888); }
    catch (const StudentNotFoundException&) { caught = true; }
    check(caught, "StudentNotFoundException : remove(8888)");

    // setGpa() invalide
    caught = false;
    try { s->setGpa(6.0f); }
    catch (const InvalidGradeException&) { caught = true; }
    check(caught, "InvalidGradeException : setGpa(6.0)");

    // setGpa() valide
    try {
        s->setGpa(3.7f);
        check(s->getGpa() == 3.7f, "setGpa(3.7) : GPA mis a jour avec succes");
    } catch (...) {
        check(false, "setGpa(3.7) ne doit pas lancer d'exception");
    }
}

// ─── Groupe 5 : Persistance & Affichage ────────────────────────────────────────
void testPersistence() {
    cout << "\n=== Groupe 5 : Persistance ===" << endl;
    const string testFile = "test_students_tmp.txt"; // Chemin portable Windows/Linux

    StudentManager mgr;
    mgr.add(make_shared<UndergraduateStudent>("Amrani Kenza",    5001, 3.8f, "Informatique"));
    mgr.add(make_shared<GraduateStudent>     ("Belhadj Youssef", 5002, 3.5f, "IA"));
    mgr.add(make_shared<PhDStudent>          ("Chaoui Leila",    5003, 3.9f, "Pr. Mansouri", 2));

    // Test de l'affichage console (display)
    cout << "--- Test d'affichage ---" << endl;
    mgr.findById(5001)->display();
    check(true, "display() s'execute sans erreur");

    // Sauvegarde
    PersistenceManager::save(mgr, testFile);
    ifstream f(testFile);
    check(f.good(), "Fichier de sauvegarde cree");
    f.close();

    // Vider la liste
    mgr.clear();
    check(mgr.count() == 0, "Liste videe avec succes");

    // Rechargement dans le meme gestionnaire
    PersistenceManager::load(mgr, testFile);
    check(mgr.count() == 3, "Rechargement : 3 etudiants retrouves");

    // Comparaison données
    auto s = mgr.findById(5001);
    check(s->getName() == "Amrani Kenza", "Persistence : nom conserve apres rechargement");
    check(s->getGpa()  == 3.8f,           "Persistence : GPA conserve apres rechargement");
    check(s->getType() == "Licence",      "Persistence : type conserve apres rechargement");

    // Fichier absent → pas de plantage
    StudentManager mgr3;
    PersistenceManager::load(mgr3, "fichier_inexistant_xyz.txt");
    check(mgr3.count() == 0, "Fichier absent : demarre avec liste vide sans crash");

    remove(testFile.c_str());
}

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════════════╗" << endl;
    cout << "║     TESTS — StudentManager C++           ║" << endl;
    cout << "╚══════════════════════════════════════════╝" << endl;

    testStudentTypes();
    testCRUD();
    testSort();
    testExceptions();
    testPersistence();

    cout << "\n╔══════════════════════════════════════════╗" << endl;
    cout << "║  RESULTATS : " << passed << "/" << total
         << " tests passes"
         << (failed ? "  ← " + to_string(failed) + " echec(s)" : "  ✓ TOUT OK")
         << endl;
    cout << "╚══════════════════════════════════════════╝" << endl;

    return failed == 0 ? 0 : 1;
}
