#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace TermColor {

class MainWindow : public QMainWindow {
    class MainWindowImpl;
public:
    MainWindow(QWidget *parent = nullptr);

private:
    MainWindowImpl *pimpl_;
};

}

#endif // MAINWINDOW_H
