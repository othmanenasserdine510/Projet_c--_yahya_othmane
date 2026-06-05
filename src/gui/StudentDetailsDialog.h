#ifndef STUDENTDETAILSDIALOG_H
#define STUDENTDETAILSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <memory>
#include "../models/Student.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"

// Fiche détaillée d'un étudiant
class StudentDetailsDialog : public QDialog {
    Q_OBJECT
public:
    StudentDetailsDialog(std::shared_ptr<Student> s, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Détails de l'étudiant");
        setMinimumWidth(350);

        QVBoxLayout* layout = new QVBoxLayout(this);

        auto addRow = [&](const QString& label, const QString& value) {
            layout->addWidget(new QLabel(QString("<b>%1</b> %2").arg(label, value)));
        };

        addRow("ID :",       QString::number(s->getId()));
        addRow("Nom :",      QString::fromStdString(s->getName()));
        addRow("Type :",     QString::fromStdString(s->getType()));
        addRow("GPA :",      QString::number(s->getGpa(), 'f', 1));
        addRow("Bourse :",   QString::number(s->computeScholarship(), 'f', 0) + " EUR");

        if (auto ug = std::dynamic_pointer_cast<UndergraduateStudent>(s))
            addRow("Filière :", QString::fromStdString(ug->getMajor()));
        else if (auto gr = std::dynamic_pointer_cast<GraduateStudent>(s))
            addRow("Sujet :", QString::fromStdString(gr->getResearchTopic()));
        else if (auto ph = std::dynamic_pointer_cast<PhDStudent>(s)) {
            addRow("Directeur :", QString::fromStdString(ph->getSupervisor()));
            addRow("Année :",     QString::number(ph->getYearOfStudy()));
        }

        auto* btns = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(btns);
    }
};

#endif // STUDENTDETAILSDIALOG_H
