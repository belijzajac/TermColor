#include "MainWindow.h"

// Models
#include "gui/model/colorsmodel/ColorsModel.h"
#include "gui/model/terminalsmodel/TerminalsModel.h"
// Views
#include "gui/widget/colorstablewidget/ColorsTableWidget.h"
#include "gui/widget/displaywidget/DisplayWidget.h"
#include "gui/widget/imagedropwidget/ImageDropWidget.h"
#include "gui/widget/exportwidget/ExportWidget.h"
#include "gui/widget/bgfgchooser/BGFGChooser.h"
// Controllers
#include "gui/controller/terminalscontroller/TerminalsController.h"
#include "gui/controller/colorscontroller/ColorsController.h"

#include <QHBoxLayout>

namespace TermColor {

// Main window (controller)
class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);

private slots:
    void undoHideWidgets();
    void onProcessColors(std::string_view imgPath);
    void onNoTerminalsFound();

private:
    void doLayout();
    void doConnections();

private:
    QHBoxLayout *layout_;

    // Models
    ColorsModel *colorsModel_;
    TerminalsModel *terminalsModel_;
    // Views
    ColorsTableWidget *colorsTableWidget_;
    DisplayWidget *displayWidget_;
    ImageDropWidget *imageDropWidget_;
    ExportWidget *exportWidget_;
    BGFGChooser *bgfgChooser_;
    // Controllers
    TerminalsController *terminalController_;
    ColorsController *colorsController_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent)
    : QWidget{parent}
{
    // models
    colorsModel_        = new ColorsModel{this};
    terminalsModel_     = new TerminalsModel{this};
    // views
    colorsTableWidget_  = new ColorsTableWidget{*colorsModel_, this};
    displayWidget_      = new DisplayWidget{*colorsModel_, this};
    imageDropWidget_    = new ImageDropWidget{this};
    exportWidget_       = new ExportWidget{*terminalsModel_, this};
    bgfgChooser_        = new BGFGChooser{this};
    // controllers
    terminalController_ = new TerminalsController{terminalsModel_, *colorsModel_, this};
    colorsController_   = new ColorsController{colorsModel_, this};

    layout_ = new QHBoxLayout{this};
    doLayout();

    doConnections();
}

void MainWindow::MainWindowImpl::doLayout() {
    layout_->addWidget(imageDropWidget_, 0, Qt::AlignCenter);
    layout_->addWidget(colorsTableWidget_, 0, Qt::AlignTop);

    // Align (and group) horizontally
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(bgfgChooser_, 0, Qt::AlignLeft);
    hLayout->addWidget(exportWidget_, 0, Qt::AlignRight);

    // displayWidget_ will be at top and everything that's in hLayout at the bottom
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(displayWidget_, 0, Qt::AlignTop);
    vLayout->addLayout(hLayout);

    // Top level layout holds all the other mini-layouts
    layout_->addLayout(vLayout);

    // By default these should be hidden as long as we don't drop
    // an image onto ImageDropWidget
    colorsTableWidget_->hide();
    displayWidget_->hide();
    exportWidget_->hide();
    bgfgChooser_->hide();
}

void MainWindow::MainWindowImpl::doConnections() {
    // Connect ImageDropWidget
    connect(imageDropWidget_, SIGNAL(imageDropped(QString)), colorsModel_, SLOT(onImageDropped(QString)));
    connect(colorsModel_, SIGNAL(hideImageDropWidget()), imageDropWidget_, SLOT(hideWidget()));
    connect(colorsModel_, SIGNAL(doProcessColors(std::string_view)), this, SLOT(onProcessColors(std::string_view)));
    connect(imageDropWidget_, SIGNAL(onHideWidget()), this, SLOT(undoHideWidgets()));

    // Connect ColorsTableWidget
    connect(colorsModel_, SIGNAL(modelChanged()), colorsTableWidget_, SLOT(onModelChanged()));

    // Connect DisplayWidget
    connect(colorsModel_, SIGNAL(modelChanged()), displayWidget_, SLOT(onModelChanged()));

    // Connect ExportWidget
    connect(terminalsModel_, SIGNAL(modelChanged()), exportWidget_, SLOT(onModelChanged()));
    connect(exportWidget_, SIGNAL(saveBtnClicked(std::string_view)), terminalController_, SLOT(onProcessSave(std::string_view)));
    connect(exportWidget_, SIGNAL(saveToJsonBtnClicked(std::string_view)), terminalController_, SLOT(onSaveToJson(std::string_view)));

    // Connect BGFGChooser
    connect(bgfgChooser_, SIGNAL(radioBtnClicked(int)), colorsController_, SLOT(onRadioBtnClicked(int)));
    connect(terminalController_, SIGNAL(noTerminalsFound()), this, SLOT(onNoTerminalsFound()));
}

void MainWindow::MainWindowImpl::undoHideWidgets() {
    colorsTableWidget_->show();
    displayWidget_->show();
    exportWidget_->show();
    bgfgChooser_->show();
}

void MainWindow::MainWindowImpl::onProcessColors(std::string_view imgPath) {
    // Update model's data with new colors
    // And available terminal emulators
    colorsController_->doImageColors(imgPath);
    terminalController_->doTerminals();
}

void MainWindow::MainWindowImpl::onNoTerminalsFound() {
    exportWidget_->disableSaveBtn();
}

// MainWindow

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(845, 500);
}

}

#include "MainWindow.moc"
