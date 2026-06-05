// main_gui.cpp — Point d'entrée pour l'interface graphique Qt
#include <QApplication>
#include "src/gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("StudentManager");
    app.setApplicationVersion("1.0");

    MainWindow window;
    window.show();

    return app.exec();
}
