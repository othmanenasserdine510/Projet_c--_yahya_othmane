#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include <fstream>
#include <string>
#include "../services/StudentManager.h"

// Export CSV compatible Excel (UTF-8 BOM, séparateur ;)
class CsvExporter {
public:
    static void exportToFile(const StudentManager& mgr, const std::string& path) {
        std::ofstream f(path);
        if (!f.is_open()) return;

        // BOM UTF-8 pour Excel
        f << "\xEF\xBB\xBF";
        f << "ID;Nom;Type;GPA;Bourse (EUR)\n";

        for (const auto& [id, s] : mgr.getAll()) {
            f << s->getId()    << ";"
              << s->getName()  << ";"
              << s->getType()  << ";"
              << s->getGpa()   << ";"
              << s->computeScholarship() << "\n";
        }
    }
};

#endif // CSVEXPORTER_H
