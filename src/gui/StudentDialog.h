#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <memory>
#include "../models/Student.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"

// Formulaire d'ajout / modification d'un étudiant
class StudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDialog(QWidget* parent = nullptr,
                           std::shared_ptr<Student> existing = nullptr);

    std::shared_ptr<Student> getStudent() const;

private slots:
    void onTypeChanged(const QString& type);
    void onValidate();

private:
    QComboBox*      comboType;
    QLineEdit*      fieldName;
    QLineEdit*      fieldId;
    QDoubleSpinBox* spinGpa;

    // Champs dynamiques
    QLineEdit*  fieldMajor;         // Licence
    QLineEdit*  fieldResearch;      // Master
    QLineEdit*  fieldSupervisor;    // Doctorat
    QSpinBox*   spinYear;           // Doctorat
    QLabel*     lblMajor;
    QLabel*     lblResearch;
    QLabel*     lblSupervisor;
    QLabel*     lblYear;

    QFormLayout* formLayout;

    std::shared_ptr<Student> result;
    bool editMode;
    int  originalId;
};

#endif // STUDENTDIALOG_H
