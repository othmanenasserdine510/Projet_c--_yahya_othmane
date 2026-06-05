#include "MainWindow.h"
#include <QApplication>
#include "StudentDialog.h"
#include "StudentDetailsDialog.h"
#include "StatsDialog.h"
#include "CsvExporter.h"
#include "../exceptions/StudentExceptions.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QString>
#include <QDir>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Recherche dynamique du fichier de données
    dataFile = "data/students.txt";
    if (!QFile::exists(dataFile)) {
        QString appDir = QCoreApplication::applicationDirPath();
        QString inProjectBuild = appDir + "/../../../data/students.txt";
        QString inShadowBuild = appDir + "/../../Projet_c++_yahya/data/students.txt";
        
        if (QFile::exists(inProjectBuild)) {
            dataFile = inProjectBuild;
        } else if (QFile::exists(inShadowBuild)) {
            dataFile = inShadowBuild;
        } else {
            QDir().mkpath("data");
        }
    }

    setWindowTitle("Gestion des Étudiants");
    setMinimumSize(900, 550);

    setupUI();
    setupMenus();

    // Chargement au démarrage
    PersistenceManager::load(manager, dataFile.toStdString());
    refreshTable();
}

MainWindow::~MainWindow() {
    PersistenceManager::save(manager, dataFile.toStdString());
}

// ─── Construction de l'UI ────────────────────────────────────────────────────
void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Barre de recherche + filtre
    QHBoxLayout* filterLayout = new QHBoxLayout;
    searchField = new QLineEdit;
    searchField->setPlaceholderText("🔍 Rechercher par nom...");
    filterType  = new QComboBox;
    filterType->addItems({"Tous", "Licence", "Master", "Doctorat"});

    filterLayout->addWidget(new QLabel("Recherche :"));
    filterLayout->addWidget(searchField, 2);
    filterLayout->addWidget(new QLabel("Type :"));
    filterLayout->addWidget(filterType, 1);
    mainLayout->addLayout(filterLayout);

    // Tableau principal
    tableWidget = new QTableWidget(0, 5, this);
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Type", "GPA", "Bourse (EUR)"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    mainLayout->addWidget(tableWidget);

    // Boutons d'action
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnAdd     = new QPushButton("＋ Ajouter");
    btnEdit    = new QPushButton("✎ Modifier");
    btnDelete  = new QPushButton("🗑 Supprimer");
    btnDetails = new QPushButton("🔍 Détails");
    btnRefresh = new QPushButton("↻ Rafraîchir");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addWidget(btnDetails);
    btnLayout->addStretch();
    btnLayout->addWidget(btnRefresh);
    mainLayout->addLayout(btnLayout);

    // Connexions signaux
    connect(btnAdd,     &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnEdit,    &QPushButton::clicked, this, &MainWindow::onEditStudent);
    connect(btnDelete,  &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnDetails, &QPushButton::clicked, this, &MainWindow::onShowDetails);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefresh);
    connect(searchField, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(filterType,  &QComboBox::currentTextChanged, this, &MainWindow::onFilterTypeChanged);
}

void MainWindow::setupMenus() {
    QMenu* fileMenu = menuBar()->addMenu("Fichier");
    fileMenu->addAction("Export CSV",    this, &MainWindow::onExportCsv);
    fileMenu->addAction("Statistiques",  this, &MainWindow::onShowStats);
    fileMenu->addSeparator();
    fileMenu->addAction("Quitter",       this, &QWidget::close);

    QMenu* viewMenu = menuBar()->addMenu("Affichage");
    viewMenu->addAction("Mode sombre / clair", this, &MainWindow::onToggleDarkMode);
}

// ─── Rafraîchissement du tableau ─────────────────────────────────────────────
void MainWindow::refreshTable() {
    auto list = getFilteredStudents();
    tableWidget->setRowCount(0);

    for (const auto& s : list) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(s->getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s->getType())));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(s->getGpa(), 'f', 1)));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(s->computeScholarship(), 'f', 0)));
    }
    updateStatusBar();
}

std::vector<std::shared_ptr<Student>> MainWindow::getFilteredStudents() const {
    std::vector<std::shared_ptr<Student>> result;
    QString query = searchField->text().trimmed().toLower();
    QString typeFilter = filterType->currentText();

    for (const auto& [id, s] : manager.getAll()) {
        if (typeFilter != "Tous" && QString::fromStdString(s->getType()) != typeFilter)
            continue;
        if (!query.isEmpty()) {
            QString name = QString::fromStdString(s->getName()).toLower();
            if (!name.contains(query)) continue;
        }
        result.push_back(s);
    }
    return result;
}

void MainWindow::updateStatusBar() {
    statusBar()->showMessage(
        QString("Étudiants : %1  |  Moyenne GPA : %2")
            .arg(manager.count())
            .arg(QString::number(manager.averageGpa(), 'f', 2))
    );
}

// ─── Slots ───────────────────────────────────────────────────────────────────
void MainWindow::onAddStudent() {
    StudentDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    try {
        manager.add(dlg.getStudent());
        PersistenceManager::save(manager, dataFile.toStdString());
        refreshTable();
        statusBar()->showMessage("Étudiant ajouté avec succès.", 3000);
    } catch (const DuplicateIDException& e) {
        QMessageBox::critical(this, "Erreur ID", QString::fromStdString(e.what()));
    } catch (const InvalidGradeException& e) {
        QMessageBox::warning(this, "Erreur GPA", QString::fromStdString(e.what()));
    } catch (const InvalidIDException& e) {
        QMessageBox::warning(this, "Erreur ID", QString::fromStdString(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromStdString(e.what()));
    }
}

void MainWindow::onEditStudent() {
    int row = tableWidget->currentRow();
    if (row < 0) { QMessageBox::information(this, "Info", "Sélectionnez un étudiant."); return; }

    int id = tableWidget->item(row, 0)->text().toInt();
    try {
        auto existing = manager.findById(id);
        StudentDialog dlg(this, existing);
        if (dlg.exec() != QDialog::Accepted) return;

        manager.update(id, dlg.getStudent());
        PersistenceManager::save(manager, dataFile.toStdString());
        refreshTable();
        statusBar()->showMessage("Étudiant modifié avec succès.", 3000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromStdString(e.what()));
    }
}

void MainWindow::onDeleteStudent() {
    int row = tableWidget->currentRow();
    if (row < 0) { QMessageBox::information(this, "Info", "Sélectionnez un étudiant."); return; }

    int id = tableWidget->item(row, 0)->text().toInt();
    auto reply = QMessageBox::question(this, "Confirmation",
        QString("Supprimer l'étudiant ID %1 ?").arg(id));
    if (reply != QMessageBox::Yes) return;

    try {
        manager.remove(id);
        PersistenceManager::save(manager, dataFile.toStdString());
        refreshTable();
        statusBar()->showMessage("Étudiant supprimé.", 3000);
    } catch (const StudentNotFoundException& e) {
        QMessageBox::warning(this, "Introuvable", QString::fromStdString(e.what()));
    }
}

void MainWindow::onRefresh() { refreshTable(); }

void MainWindow::onSearchChanged(const QString&)    { refreshTable(); }
void MainWindow::onFilterTypeChanged(const QString&){ refreshTable(); }

void MainWindow::onShowDetails() {
    int row = tableWidget->currentRow();
    if (row < 0) { QMessageBox::information(this, "Info", "Sélectionnez un étudiant."); return; }

    int id = tableWidget->item(row, 0)->text().toInt();
    try {
        auto s = manager.findById(id);
        StudentDetailsDialog dlg(s, this);
        dlg.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromStdString(e.what()));
    }
}

void MainWindow::onExportCsv() {
    QString path = QFileDialog::getSaveFileName(this, "Exporter CSV", "etudiants.csv",
                                                "CSV (*.csv)");
    if (path.isEmpty()) return;
    CsvExporter::exportToFile(manager, path.toStdString());
    statusBar()->showMessage("Export CSV réussi : " + path, 4000);
}

void MainWindow::onShowStats() {
    StatsDialog dlg(manager, this);
    dlg.exec();
}

void MainWindow::onToggleDarkMode() {
    darkMode = !darkMode;
    if (darkMode) {
        qApp->setStyleSheet(
            "QWidget { background-color: #2b2b2b; color: #f0f0f0; }"
            "QTableWidget { gridline-color: #555; alternate-background-color: #333; }"
            "QPushButton { background-color: #3c3f41; border: 1px solid #666; padding: 4px 8px; }"
            "QPushButton:hover { background-color: #4e5254; }"
            "QLineEdit, QComboBox { background-color: #3c3f41; border: 1px solid #666; }"
            "QMenuBar { background-color: #3c3f41; }"
            "QStatusBar { background-color: #3c3f41; }"
        );
    } else {
        qApp->setStyleSheet("");
    }
}
