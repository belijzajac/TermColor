#include "MainWindow.h"
#include "ColorsTableWidget.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <memory>
#include "GuiModel.h"

#include <QLabel>

class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);

private:
    void doLayout();
    //void connectInputToModel();

    MainWindow &parent_;
    ColorsTableWidget *colorsTableWidget_;
    GuiModel *guiModel_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent}, parent_{*parent} {
    guiModel_          = new GuiModel{this};
    colorsTableWidget_ = new ColorsTableWidget{this}; // input widget

    doLayout();

    //connectInputToModel();
}

void MainWindow::MainWindowImpl::doLayout() {
    auto hblayout = new QHBoxLayout{this};
    // add display widget

    auto label = new QLabel{this};
    label->setText("text text text");

    hblayout->addWidget(colorsTableWidget_);
    hblayout->addWidget(label);
}

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(QSize{500,500});
}

#include "MainWindow.moc"
