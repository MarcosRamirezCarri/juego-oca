#include <QApplication>
#include "MainWindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}


