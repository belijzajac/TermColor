#include "MainWindow.h"
#include <ui/gui/colorstablewidget/ColorsTableWidget.h>
#include <ui/gui/displaywidget/DisplayWidget.h>
#include <ui/gui/guimodel/GuiModel.h>
#include <ui/gui/imagedropwidget/ImageDropWidget.h>
#include <QHBoxLayout>
#include <memory>

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
    ImageDropWidget *imageDropWidget_;
    GuiModel *guiModel_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent}, parent_{*parent} {
    guiModel_          = new GuiModel{this};
    colorsTableWidget_ = new ColorsTableWidget{this};
    displayWidget_     = new DisplayWidget{this};
    imageDropWidget_   = new ImageDropWidget{this};

    layout_ = new QHBoxLayout{this};
    doLayout();

    colorsTableWidget_->hide();
    displayWidget_->hide();

    //connectInputToModel();
}

void MainWindow::MainWindowImpl::doLayout() {
    layout_->addWidget(imageDropWidget_, 0, Qt::AlignCenter);
    layout_->addWidget(colorsTableWidget_, 0, Qt::AlignTop);
    layout_->addWidget(displayWidget_, 0, Qt::AlignTop);
}

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(500, 500);
    //setFixedSize(pimpl_->getLayout()->totalMinimumSize());
}

#include "MainWindow.moc"
