#include "StudentDialog.h"
#include "../exceptions/StudentExceptions.hpp"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSpinBox>

StudentDialog::StudentDialog(QWidget* parent, std::shared_ptr<Student> existing)
    : QDialog(parent), editMode(existing != nullptr), originalId(0)
{
    setWindowTitle(editMode ? "Modifier un étudiant" : "Ajouter un étudiant");
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    formLayout = new QFormLayout;

    // Type
    comboType = new QComboBox;
    comboType->addItems({"Licence", "Master", "Doctorat"});
    formLayout->addRow("Type :", comboType);

    // Nom & ID
    fieldName = new QLineEdit;
    fieldId   = new QLineEdit;
    if (editMode) fieldId->setEnabled(false);
    formLayout->addRow("Nom :", fieldName);
    formLayout->addRow("ID  :", fieldId);

    // GPA
    spinGpa = new QDoubleSpinBox;
    spinGpa->setRange(0.0, 4.0);
    spinGpa->setSingleStep(0.1);
    spinGpa->setDecimals(1);
    formLayout->addRow("GPA :", spinGpa);

    // Champs spécifiques
    lblMajor      = new QLabel("Filière :");
    fieldMajor    = new QLineEdit;
    lblResearch   = new QLabel("Sujet de recherche :");
    fieldResearch = new QLineEdit;
    lblSupervisor = new QLabel("Directeur de thèse :");
    fieldSupervisor = new QLineEdit;
    lblYear       = new QLabel("Année de doctorat :");
    spinYear      = new QSpinBox;
    spinYear->setRange(1, 6);

    formLayout->addRow(lblMajor,      fieldMajor);
    formLayout->addRow(lblResearch,   fieldResearch);
    formLayout->addRow(lblSupervisor, fieldSupervisor);
    formLayout->addRow(lblYear,       spinYear);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &StudentDialog::onValidate);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(comboType, &QComboBox::currentTextChanged, this, &StudentDialog::onTypeChanged);

    // Pré-remplissage en mode édition
    if (editMode) {
        originalId = existing->getId();
        fieldName->setText(QString::fromStdString(existing->getName()));
        fieldId->setText(QString::number(existing->getId()));
        spinGpa->setValue(existing->getGpa());
        comboType->setCurrentText(QString::fromStdString(existing->getType()));

        if (auto ug = std::dynamic_pointer_cast<UndergraduateStudent>(existing))
            fieldMajor->setText(QString::fromStdString(ug->getMajor()));
        else if (auto gr = std::dynamic_pointer_cast<GraduateStudent>(existing))
            fieldResearch->setText(QString::fromStdString(gr->getResearchTopic()));
        else if (auto ph = std::dynamic_pointer_cast<PhDStudent>(existing)) {
            fieldSupervisor->setText(QString::fromStdString(ph->getSupervisor()));
            spinYear->setValue(ph->getYearOfStudy());
        }
    }

    onTypeChanged(comboType->currentText());
}

void StudentDialog::onTypeChanged(const QString& type) {
    bool isLicence  = (type == "Licence");
    bool isMaster   = (type == "Master");
    bool isDoctorat = (type == "Doctorat");

    lblMajor->setVisible(isLicence);       fieldMajor->setVisible(isLicence);
    lblResearch->setVisible(isMaster);     fieldResearch->setVisible(isMaster);
    lblSupervisor->setVisible(isDoctorat); fieldSupervisor->setVisible(isDoctorat);
    lblYear->setVisible(isDoctorat);       spinYear->setVisible(isDoctorat);
    adjustSize();
}

void StudentDialog::onValidate() {
    try {
        std::string name  = fieldName->text().trimmed().toStdString();
        int         id    = fieldId->text().trimmed().toInt();
        float       gpa   = (float)spinGpa->value();
        std::string type  = comboType->currentText().toStdString();

        if (name.empty()) { QMessageBox::warning(this, "Erreur", "Le nom est obligatoire."); return; }
        if (!editMode && id < 1000) { QMessageBox::warning(this, "Erreur", "L'ID doit être >= 1000."); return; }

        if (type == "Licence") {
            std::string major = fieldMajor->text().trimmed().toStdString();
            result = std::make_shared<UndergraduateStudent>(name, id, gpa, major);
        } else if (type == "Master") {
            std::string topic = fieldResearch->text().trimmed().toStdString();
            result = std::make_shared<GraduateStudent>(name, id, gpa, topic);
        } else {
            std::string sup  = fieldSupervisor->text().trimmed().toStdString();
            int         year = spinYear->value();
            result = std::make_shared<PhDStudent>(name, id, gpa, sup, year);
        }
        accept();
    } catch (const InvalidIDException& e) {
        QMessageBox::warning(this, "Erreur ID",  QString::fromStdString(e.what()));
    } catch (const InvalidGradeException& e) {
        QMessageBox::warning(this, "Erreur GPA", QString::fromStdString(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur",    QString::fromStdString(e.what()));
    }
}

std::shared_ptr<Student> StudentDialog::getStudent() const { return result; }
