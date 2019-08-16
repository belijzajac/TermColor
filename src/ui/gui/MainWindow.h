#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    class MainWindowImpl;
public:
    MainWindow(int argc, char *argv[], QWidget *parent = nullptr);

private:
    MainWindowImpl *pimpl_;
};

#endif // MAINWINDOW_H
