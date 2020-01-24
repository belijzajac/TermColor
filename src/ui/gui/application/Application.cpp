#include "Application.h"
#include <QMessageBox>

namespace TermColor {

Application::Application(int &argc, char *argv[]) : QApplication(argc, argv) {}

int Application::run() {
    try {
        int code = proc();
        // Check if an exception was thrown and stored in Application::notify
        // and if so, rethrow it.
        if (_M_e)
            std::rethrow_exception(_M_e);
        return code;
    }
    catch (std::exception &e) {
        return message(e.what());
    }
}

int Application::message(const std::string &message) {
    QMessageBox::critical(nullptr, "Error", QString::fromStdString(message));
    return 1;
}

///
/// Qt does not allow exceptions thrown from event handlers
/// to be processed outside the event loop.
/// So we catch them here, store them in _M_e
/// and tell the application to exit.
///
bool Application::notify(QObject *receiver, QEvent *event) {
    try {
        return QApplication::notify(receiver, event);
    }
    catch (...) {
        _M_e = std::current_exception();
        exit();
    }
    return false;
}

}
