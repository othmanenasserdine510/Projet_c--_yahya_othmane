QT += core gui widgets charts

CONFIG += c++17

TARGET   = StudentManagementApp
TEMPLATE = app

SOURCES += \
    main_gui.cpp \
    src/gui/MainWindow.cpp \
    src/gui/StudentDialog.cpp

HEADERS += \
    src/models/Student.h \
    src/models/UndergraduateStudent.h \
    src/models/GraduateStudent.h \
    src/models/PhDStudent.h \
    src/services/StudentManager.h \
    src/exceptions/StudentExceptions.hpp \
    src/persistence/PersistenceManager.h \
    src/gui/MainWindow.h \
    src/gui/StudentDialog.h \
    src/gui/StudentDetailsDialog.h \
    src/gui/StatsDialog.h \
    src/gui/CsvExporter.h

# Crée le dossier data si absent
DEFINES += QT_DEPRECATED_WARNINGS
