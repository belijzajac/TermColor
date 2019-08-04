#include "MainWindow.h"
#include "ColorsTableWidget.h"
#include <QVBoxLayout>
#include <memory>
#include "GuiModel.h"

class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);

private:
    void doLayout();

    MainWindow &parent_;
    ColorsTableWidget *colorsTableWidget_;
    GuiModel *guiModel_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent}, parent_{*parent} {
    guiModel_          = new GuiModel{this};
    colorsTableWidget_ = new ColorsTableWidget{this};

    doLayout();
}

void MainWindow::MainWindowImpl::doLayout() {
    auto vblayout = new QVBoxLayout{this};
    vblayout->addWidget(colorsTableWidget_);
}

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
}

#include "MainWindow.moc"
