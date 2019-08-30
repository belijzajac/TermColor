#include <ui/gui/application/Application.h>
#include <ui/gui/MainWindow.h>

int main(int argc, char *argv[]) {
    // Create application instance
    Application app{argc, argv};
    MainWindow gui{argc, argv};

    // Set application metadata
    QCoreApplication::setApplicationName("TermColor");

    gui.show();
    app.run();

    return 0;
}
