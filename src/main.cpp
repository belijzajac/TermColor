#include <QApplication>
#include <ui/gui/MainWindow.h>

int main(int argc, char *argv[]) {
    // Create application instance
    QApplication app{argc, argv};
    MainWindow gui{argc, argv};

    // Set application metadata
    QCoreApplication::setApplicationName("TermColor");

    gui.show();

    return app.exec();
}
