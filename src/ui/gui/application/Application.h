#ifndef APPLICATION_H
#define APPLICATION_H

// Thanks Innocent Bystander @ stackoverflow.com for this example

#include <QApplication>
#include <exception>

///
/// This class catches any exceptions thrown inside proc()
/// and shows them using the message() function.
/// The virtual message() function can be overridden to customize
/// how the messages are shown. The default implementation
/// shows them using QMessageBox.
///

namespace TermColor {

class Application : public QApplication {
public:
    Application(int &argc, char *argv[]);
    bool notify(QObject *receiver, QEvent *event) override;

    virtual int proc() { return exec(); }
    int run();

    virtual int message(std::string_view);

private:
    std::exception_ptr _M_e{nullptr};
};

}

#endif // APPLICATION_H
