#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "../services/StudentManager.h"

using namespace std;

// Gère la sauvegarde et le chargement des étudiants depuis un fichier texte
// Format : Type|ID|Nom|GPA|champ_specifique[|champ2]
class PersistenceManager {
public:
    // Sauvegarde tous les étudiants dans le fichier (écrase le contenu existant)
    static void save(const StudentManager& mgr, const string& path) {
        ofstream file(path);
        if (!file.is_open()) {
            cerr << "[PersistenceManager] Impossible d'ouvrir le fichier : " << path << endl;
            return;
        }

        for (const auto& [id, s] : mgr.getAll()) {
            const string type = s->getType();
            file << type << "|" << s->getId() << "|" << s->getName() << "|" << s->getGpa();

            if (type == "Licence") {
                auto ug = dynamic_pointer_cast<UndergraduateStudent>(s);
                file << "|" << ug->getMajor();
            } else if (type == "Master") {
                auto gr = dynamic_pointer_cast<GraduateStudent>(s);
                file << "|" << gr->getResearchTopic();
            } else if (type == "Doctorat") {
                auto ph = dynamic_pointer_cast<PhDStudent>(s);
                file << "|" << ph->getSupervisor() << "|" << ph->getYearOfStudy();
            }
            file << "\n";
        }
    }

    // Charge les étudiants depuis le fichier dans le gestionnaire
    // Les lignes corrompues sont ignorées silencieusement
    static void load(StudentManager& mgr, const string& path) {
        ifstream file(path);
        if (!file.is_open()) return; // Fichier absent → liste vide, pas de plantage

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                istringstream ss(line);
                string token;
                vector<string> tokens;

                while (getline(ss, token, '|'))
                    tokens.push_back(token);

                if (tokens.size() < 5) continue; // Ligne incomplète

                string type = tokens[0];
                int    id   = stoi(tokens[1]);
                string name = tokens[2];
                float  gpa  = stof(tokens[3]);

                if (type == "Licence") {
                    mgr.add(make_shared<UndergraduateStudent>(name, id, gpa, tokens[4]));
                } else if (type == "Master") {
                    mgr.add(make_shared<GraduateStudent>(name, id, gpa, tokens[4]));
                } else if (type == "Doctorat" && tokens.size() >= 6) {
                    int year = stoi(tokens[5]);
                    mgr.add(make_shared<PhDStudent>(name, id, gpa, tokens[4], year));
                }
            } catch (...) {
                // Ligne corrompue ignorée silencieusement
            }
        }
    }
};

#endif // PERSISTENCEMANAGER_H
