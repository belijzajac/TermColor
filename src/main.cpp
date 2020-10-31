#include "gui/utils/Application.h"
#include "gui/MainWindow.h"

using namespace TermColor;

int main(int argc, char *argv[]) {
    // Create application instance
    Application app{argc, argv};
    MainWindow gui{};

    // Set application metadata
    QCoreApplication::setApplicationName("TermColor");

    gui.show();
    app.run();

    return 0;
}
