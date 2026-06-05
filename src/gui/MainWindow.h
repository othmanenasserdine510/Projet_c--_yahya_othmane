#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QComboBox>
#include <memory>
#include "../services/StudentManager.h"
#include "../persistence/PersistenceManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onRefresh();
    void onSearchChanged(const QString& text);
    void onFilterTypeChanged(const QString& type);
    void onShowDetails();
    void onExportCsv();
    void onShowStats();
    void onToggleDarkMode();

private:
    void setupUI();
    void setupMenus();
    void refreshTable();
    void updateStatusBar();
    std::vector<std::shared_ptr<Student>> getFilteredStudents() const;

    // Widgets
    QTableWidget* tableWidget;
    QPushButton*  btnAdd;
    QPushButton*  btnEdit;
    QPushButton*  btnDelete;
    QPushButton*  btnRefresh;
    QPushButton*  btnDetails;
    QLineEdit*    searchField;
    QComboBox*    filterType;

    // Données
    StudentManager manager;
    QString        dataFile;
    bool           darkMode = false;
};

#endif // MAINWINDOW_H
