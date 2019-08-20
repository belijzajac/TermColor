#include "MainWindow.h"
#include <ui/gui/colorstablewidget/ColorsTableWidget.h>
#include <ui/gui/displaywidget/DisplayWidget.h>
#include <ui/gui/guimodel/GuiModel.h>
#include <ui/gui/imagedropwidget/ImageDropWidget.h>
#include <ui/gui/exportwidget/ExportWidget.h>
#include <backend/writer/Writer.h>
#include <backend/writer/konsolewriter/KonsoleWriter.h>
#include <QHBoxLayout>
#include <filesystem>
#include <memory>

// Main window (controller)
class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);
    QHBoxLayout *getLayout() { return layout_; }

private slots:
    void undoHideWidgets();
    void onProcessColors(const std::string &imgPath);
    void onProcessSave(const std::string &saveOption);

private:
    void doLayout();
    void doConnections();

    // Functions to update model's data
    void doImageColors(const std::string &imgPath);
    void doTerminals();

private:
    QHBoxLayout *layout_;
    MainWindow &parent_;

    // Views
    ColorsTableWidget *colorsTableWidget_;
    DisplayWidget *displayWidget_;
    ImageDropWidget *imageDropWidget_;
    ExportWidget *exportWidget_;

    // Model
    GuiModel *guiModel_;

    // Where the logic about image processing lives
    std::unique_ptr<DominantColor> domColor;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent}, parent_{*parent} {
    guiModel_          = new GuiModel{this};
    colorsTableWidget_ = new ColorsTableWidget{*guiModel_, this};
    displayWidget_     = new DisplayWidget{*guiModel_, this};
    imageDropWidget_   = new ImageDropWidget{this};
    exportWidget_      = new ExportWidget{*guiModel_, this};
    domColor           = std::make_unique<DominantColor>();

    layout_ = new QHBoxLayout{this};
    doLayout();

    doConnections();
}

void MainWindow::MainWindowImpl::doLayout() {
    auto vLayout = new QVBoxLayout;

    layout_->addWidget(imageDropWidget_, 0, Qt::AlignCenter);
    layout_->addWidget(colorsTableWidget_, 0, Qt::AlignTop);

    vLayout->addWidget(displayWidget_, 0, Qt::AlignTop);
    vLayout->addWidget(exportWidget_, 0, Qt::AlignRight);

    layout_->addLayout(vLayout);

    // By default these should be hidden as long as we don't drop
    // an image onto ImageDropWidget
    colorsTableWidget_->hide();
    displayWidget_->hide();
    exportWidget_->hide();
}

void MainWindow::MainWindowImpl::doConnections() {
    // Connect ImageDropWidget (view) to model
    connect(imageDropWidget_, SIGNAL(imageDropped(QString)), guiModel_, SLOT(onImageDropped(QString)));
    connect(guiModel_, SIGNAL(hideImageDropWidget()), imageDropWidget_, SLOT(hideWidget()));
    connect(guiModel_, SIGNAL(doProcessColors(std::string)), this, SLOT(onProcessColors(std::string)));

    // Connect ImageDropWidget (view) to controller
    connect(imageDropWidget_, SIGNAL(onHideWidget()), this, SLOT(undoHideWidgets()));

    // Connect ColorsTableWidget (view) to model
    connect(guiModel_, SIGNAL(modelChanged()), colorsTableWidget_, SLOT(onModelChanged()));

    // Connect DisplayWidget (view) to model
    connect(guiModel_, SIGNAL(modelChanged()), displayWidget_, SLOT(onModelChanged()));

    // Connect ExportWidget (view) to model and back to controller (here), to do the logic
    connect(guiModel_, SIGNAL(modelChanged()), exportWidget_, SLOT(onModelChanged()));
    connect(exportWidget_, SIGNAL(saveBtnClicked(std::string)), this, SLOT(onProcessSave(std::string)));
}

void MainWindow::MainWindowImpl::undoHideWidgets() {
    colorsTableWidget_->show();
    displayWidget_->show();
    exportWidget_->show();
}

void MainWindow::MainWindowImpl::doImageColors(const std::string &imgPath) {
    // Passes an image to opencv's cv::imread function
    // and later performs the k-means' algorithm
    domColor->readImage(imgPath);
    domColor->performKMeans();

    // Obtains extracted colors
    auto dominantColors = domColor->getColors();
    auto intenseColors = domColor->intenseColors(dominantColors);

    // Obtain predefined BG/FG colors
    auto bgfg = domColor->getBGFGColors();
    auto bgfgIntense = domColor->intenseColors(bgfg);

    // Copy colors to one continuous vector<color> DS
    dominantColors.insert(dominantColors.end(), intenseColors.begin(), intenseColors.end());
    bgfg.insert(bgfg.end(), bgfgIntense.begin(), bgfgIntense.end());

    // Populates model with new data (colors, in this case)
    guiModel_->setImgColors(dominantColors);
    guiModel_->setBGFGColors(bgfg);
}

void MainWindow::MainWindowImpl::doTerminals() {
    const GuiModel::Terminals &terminals = guiModel_->getTerminals();
    auto supportedTerminals = terminals.supported_;

    // Find supported terminals in /bin
    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator("/bin")) {
        const auto strEntry = dirEntry.path().string(); // e.g. "/bin/konsole"

        // Loop through terminals
        for (auto it = supportedTerminals.begin(); it != supportedTerminals.end();) {
            // We compare the length in case of an entry such as "/bin/konsoleprofile" if we
            // only need "/bin/konsole". Then we compare its substring against supported terminals
            if (strEntry.length() - 5 == it->length() && strEntry.substr(5, it->length()) == *it) {
                // Update model and remove already found entry
                guiModel_->insertTerminal(*it);
                it = supportedTerminals.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void MainWindow::MainWindowImpl::onProcessColors(const std::string &imgPath) {
    // Update model's data with new colors
    // And available terminal emulators
    doImageColors(imgPath);
    doTerminals();

    emit guiModel_->modelChanged();
}

void MainWindow::MainWindowImpl::onProcessSave(const std::string &saveOption) {
    const auto termType = GuiModel::terminalToEnum_[saveOption];

    // A functional approach in applying the Factory design pattern to construct a writer
    const auto writer = [termType] {
        switch (termType) {
            case TerminalType::Konsole:
                return std::make_unique<KonsoleWriter>();
            case TerminalType::GnomeTerminal:
                std::make_unique<KonsoleWriter>();
        }
    }();

    const GuiModel::Colors &colors = guiModel_->getColors();
    writer->writeToLocation("NEW_NEW_", colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);
}

// MainWindow

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(845, 500);
}

#include "MainWindow.moc"
