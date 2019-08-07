#include "MainWindow.h"
#include "ColorsTableWidget.h"
#include "DisplayWidget.h"
#include <QHBoxLayout>
#include <memory>
#include "GuiModel.h"

class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);
    QHBoxLayout *getLayout() { return layout_; }

private:
    void doLayout();
    //void connectInputToModel();

    QHBoxLayout *layout_;

    MainWindow &parent_;
    ColorsTableWidget *colorsTableWidget_;
    DisplayWidget *displayWidget_;
    GuiModel *guiModel_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent}, parent_{*parent} {
    guiModel_          = new GuiModel{this};
    colorsTableWidget_ = new ColorsTableWidget{this};
    displayWidget_     = new DisplayWidget{this};

    layout_ = new QHBoxLayout{this};
    doLayout();

    //connectInputToModel();
}

void MainWindow::MainWindowImpl::doLayout() {
    layout_->addWidget(colorsTableWidget_, 0, Qt::AlignTop);
    layout_->addWidget(displayWidget_, 0, Qt::AlignTop);
}

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(pimpl_->getLayout()->totalMinimumSize());
}

#include "MainWindow.moc"
