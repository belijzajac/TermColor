#include "backend/dominantcolor/DominantColor.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    // Create application instance
    QApplication a(argc, argv);

    // Set application metadata
    QCoreApplication::setApplicationName("TermColor");

    //...

    return a.exec();
}
