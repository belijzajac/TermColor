#include "MainWindow.h"
#include <ui/gui/widget/colorstablewidget/ColorsTableWidget.h>
#include <ui/gui/widget/displaywidget/DisplayWidget.h>
#include <ui/gui/model/colorsmodel/ColorsModel.h>
#include <ui/gui/model/terminalsmodel/TerminalsModel.h>
#include <ui/gui/widget/imagedropwidget/ImageDropWidget.h>
#include <ui/gui/widget/exportwidget/ExportWidget.h>
#include <ui/gui/widget/bgfgchooser/BGFGChooser.h>
#include <backend/writer/Writer.h>
#include <backend/writer/konsolewriter/KonsoleWriter.h>
#include <backend/writer/xfce4terminalwriter/Xfce4TerminalWriter.h>
#include <backend/writer/lxterminalwriter/LXTerminalWriter.h>
#include <backend/writer/jsonwriter/JsonWriter.h>
#include <backend/exception/Exception.h>
#include <QHBoxLayout>
#include <filesystem>
#include <memory>
#include <ctime>

// Main window (controller)
class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);

private slots:
    void undoHideWidgets();
    void onProcessColors(const std::string &imgPath);
    void onProcessSave(const std::string &saveOption);
    void onSaveToJson(const std::string &saveLocation);
    void onRadioBtnClicked(int id);

private:
    void doLayout();
    void doConnections();

    // Functions to update model's data
    void doImageColors(const std::string &imgPath);
    void doTerminals();

    // Construct appropriate terminal writer
    const std::unique_ptr<Writer> writerFactory(TerminalsModel::TerminalType t) const;

private:
    QHBoxLayout *layout_;

    // Views
    ColorsTableWidget *colorsTableWidget_;
    DisplayWidget *displayWidget_;
    ImageDropWidget *imageDropWidget_;
    ExportWidget *exportWidget_;
    BGFGChooser *bgfgChooser_;

    // Models
    ColorsModel *colorsModel_;
    TerminalsModel *terminalsModel_;

    // Where the logic about image processing lives
    std::unique_ptr<DominantColor> domColor;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent) : QWidget{parent} {
    colorsModel_       = new ColorsModel{this};
    terminalsModel_    = new TerminalsModel{this};
    colorsTableWidget_ = new ColorsTableWidget{*colorsModel_, this};
    displayWidget_     = new DisplayWidget{*colorsModel_, this};
    imageDropWidget_   = new ImageDropWidget{this};
    exportWidget_      = new ExportWidget{*terminalsModel_, this};
    bgfgChooser_       = new BGFGChooser{this};
    domColor           = std::make_unique<DominantColor>();

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
    // Connect ImageDropWidget (view) to model
    connect(imageDropWidget_, SIGNAL(imageDropped(QString)), colorsModel_, SLOT(onImageDropped(QString)));
    connect(colorsModel_, SIGNAL(hideImageDropWidget()), imageDropWidget_, SLOT(hideWidget()));
    connect(colorsModel_, SIGNAL(doProcessColors(std::string)), this, SLOT(onProcessColors(std::string)));

    // Connect ImageDropWidget (view) to controller
    connect(imageDropWidget_, SIGNAL(onHideWidget()), this, SLOT(undoHideWidgets()));

    // Connect ColorsTableWidget (view) to model
    connect(colorsModel_, SIGNAL(modelChanged()), colorsTableWidget_, SLOT(onModelChanged()));

    // Connect DisplayWidget (view) to model
    connect(colorsModel_, SIGNAL(modelChanged()), displayWidget_, SLOT(onModelChanged()));

    // Connect ExportWidget (view) to model and back to controller (here), to do the logic
    connect(terminalsModel_, SIGNAL(modelChanged()), exportWidget_, SLOT(onModelChanged()));
    connect(exportWidget_, SIGNAL(saveBtnClicked(std::string)), this, SLOT(onProcessSave(std::string)));
    connect(exportWidget_, SIGNAL(saveToJsonBtnClicked(std::string)), this, SLOT(onSaveToJson(std::string)));

    // Connect BGFGChooser (view) to controller
    connect(bgfgChooser_, SIGNAL(radioBtnClicked(int)), this, SLOT(onRadioBtnClicked(int)));
}

void MainWindow::MainWindowImpl::undoHideWidgets() {
    colorsTableWidget_->show();
    displayWidget_->show();
    exportWidget_->show();
    bgfgChooser_->show();
}

void MainWindow::MainWindowImpl::doImageColors(const std::string &imgPath) {
    try {
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
        colorsModel_->setImgColors(dominantColors);
        colorsModel_->setBGFGColors(bgfg);

    } catch (Exception &e) {
        throw;
    }
}

void MainWindow::MainWindowImpl::doTerminals() {
    const TerminalsModel::Terminals &terminals = terminalsModel_->getTerminals();
    auto supportedTerminals = terminals.supported_;
    std::vector<std::string> terminalsFound; // Found terminals in /bin

    // Find supported terminals in /bin
    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator("/bin")) {
        const auto strEntry = dirEntry.path().string(); // e.g. "/bin/konsole"

        // Loop through terminals
        for (auto it = supportedTerminals.begin(); it != supportedTerminals.end();) {
            // We compare the length in case of an entry such as "/bin/konsoleprofile" if we
            // only need "/bin/konsole". Then we compare its substring against supported terminals
            if (strEntry.length() - 5 == it->length() && strEntry.substr(5, it->length()) == *it) {
                // Append list of found terminals and remove already found entry
                terminalsFound.push_back(*it);
                it = supportedTerminals.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Check if there are any found terminals
    if(terminalsFound.empty()) {
        exportWidget_->disableSaveBtn();
    } else {
        // Populate model with new terminals
        terminalsModel_->insertTerminals(terminalsFound);
    }
}

void MainWindow::MainWindowImpl::onProcessColors(const std::string &imgPath) {
    // Update model's data with new colors
    // And available terminal emulators
    doImageColors(imgPath);
    doTerminals();
}

const std::string currentTimestamp() {
    std::time_t  time = std::time(nullptr);
    std::tm *timeNow  = std::localtime(&time);

    return std::to_string(timeNow->tm_year + 1900) + '-' +
           std::to_string(timeNow->tm_mon + 1) + '-' +
           std::to_string(timeNow->tm_mday) + "-" +
           std::to_string(timeNow->tm_hour) + "-" +
           std::to_string(timeNow->tm_min) + "-" +
           std::to_string(timeNow->tm_sec);
}

void MainWindow::MainWindowImpl::onProcessSave(const std::string &saveOption) {
    try {
        const auto termType = TerminalsModel::terminalToEnum_[saveOption];

        // Construct a writer
        const auto writer = writerFactory(termType);

        const ColorsModel::Colors &colors = colorsModel_->getColors();
        writer->writeToLocation(currentTimestamp(), colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);
    } catch (Exception &e) {
        throw;
    }
}

void MainWindow::MainWindowImpl::onSaveToJson(const std::string &saveLocation) {
    const auto jsonWriter = std::make_unique<JsonWriter>();
    const ColorsModel::Colors &colors = colorsModel_->getColors();
    jsonWriter->writeToLocation(saveLocation, colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);
}

void MainWindow::MainWindowImpl::onRadioBtnClicked(int id) {
    // Dark BG/FG (id = 0) or Light BG/FG (id = 1)
    // getBGFGColors(true) ==> obtain dark BG/FG, otherwise (if false) - light BG/FG
    auto bgfg = domColor->getBGFGColors(id == 0);
    const auto bgfgIntense = domColor->intenseColors(bgfg);

    // Copy colors to one continuous vector<color> DS
    bgfg.insert(bgfg.end(), bgfgIntense.begin(), bgfgIntense.end());

    // Populates model with new data (colors, in this case)
    colorsModel_->setBGFGColors(bgfg);
}

const std::unique_ptr<Writer> MainWindow::MainWindowImpl::writerFactory(TerminalsModel::TerminalType t) const {
    switch (t) {
        case TerminalsModel::TerminalType::Konsole:
            return std::make_unique<KonsoleWriter>();
        case TerminalsModel::TerminalType::Xfce4Terminal:
            return std::make_unique<Xfce4TerminalWriter>();
        case TerminalsModel::TerminalType::LXTerminal:
            return std::make_unique<LXTerminalWriter>();
    }
}

// MainWindow

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(845, 500);
}

#include "MainWindow.moc"
