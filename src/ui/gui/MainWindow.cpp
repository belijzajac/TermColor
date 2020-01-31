#include "MainWindow.h"

// Views
#include <ui/gui/widget/colorstablewidget/ColorsTableWidget.h>
#include <ui/gui/widget/displaywidget/DisplayWidget.h>
#include <ui/gui/model/colorsmodel/ColorsModel.h>
#include <ui/gui/model/terminalsmodel/TerminalsModel.h>
#include <ui/gui/widget/imagedropwidget/ImageDropWidget.h>
#include <ui/gui/widget/exportwidget/ExportWidget.h>
#include <ui/gui/widget/bgfgchooser/BGFGChooser.h>

// Backend
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

namespace TermColor {

using namespace TermColor::Utils;

// Main window (controller)
class MainWindow::MainWindowImpl : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow *parent);

private slots:
    void undoHideWidgets();
    void onProcessColors(std::string_view imgPath);
    void onProcessSave(std::string_view saveOption);
    void onSaveToJson(std::string_view saveLocation);
    void onRadioBtnClicked(int id);

private:
    void doLayout();
    void doConnections();

    // Functions to update model's data
    void doImageColors(std::string_view imgPath);
    void doTerminals();

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

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow *parent)
    : QWidget{parent}
    , domColor(std::make_unique<DominantColor>())
{
    colorsModel_       = new ColorsModel{this};
    terminalsModel_    = new TerminalsModel{this};
    colorsTableWidget_ = new ColorsTableWidget{*colorsModel_, this};
    displayWidget_     = new DisplayWidget{*colorsModel_, this};
    imageDropWidget_   = new ImageDropWidget{this};
    exportWidget_      = new ExportWidget{*terminalsModel_, this};
    bgfgChooser_       = new BGFGChooser{this};

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
    connect(colorsModel_, SIGNAL(doProcessColors(std::string_view)), this, SLOT(onProcessColors(std::string_view)));

    // Connect ImageDropWidget (view) to controller
    connect(imageDropWidget_, SIGNAL(onHideWidget()), this, SLOT(undoHideWidgets()));

    // Connect ColorsTableWidget (view) to model
    connect(colorsModel_, SIGNAL(modelChanged()), colorsTableWidget_, SLOT(onModelChanged()));

    // Connect DisplayWidget (view) to model
    connect(colorsModel_, SIGNAL(modelChanged()), displayWidget_, SLOT(onModelChanged()));

    // Connect ExportWidget (view) to model and back to controller (here), to do the logic
    connect(terminalsModel_, SIGNAL(modelChanged()), exportWidget_, SLOT(onModelChanged()));
    connect(exportWidget_, SIGNAL(saveBtnClicked(std::string_view)), this, SLOT(onProcessSave(std::string_view)));
    connect(exportWidget_, SIGNAL(saveToJsonBtnClicked(std::string_view)), this, SLOT(onSaveToJson(std::string_view)));

    // Connect BGFGChooser (view) to controller
    connect(bgfgChooser_, SIGNAL(radioBtnClicked(int)), this, SLOT(onRadioBtnClicked(int)));
}

void MainWindow::MainWindowImpl::undoHideWidgets() {
    colorsTableWidget_->show();
    displayWidget_->show();
    exportWidget_->show();
    bgfgChooser_->show();
}

void MainWindow::MainWindowImpl::doImageColors(std::string_view imgPath) {
    try {
        // Passes an image to opencv's cv::imread function
        // and later performs the k-means' algorithm
        domColor->readImage(imgPath);
        domColor->performKMeans();

        // Obtains extracted colors
        auto dominantColors = domColor->getColors();
        const auto intenseColors = domColor->intenseColors(dominantColors);

        // Obtain predefined BG/FG colors
        auto bgfg = domColor->getBGFGColors();
        const auto bgfgIntense = domColor->intenseColors(bgfg);

        // Copy colors to one continuous vector<color> DS
        dominantColors.insert(dominantColors.end(), intenseColors.begin(), intenseColors.end());
        bgfg.insert(bgfg.end(), bgfgIntense.begin(), bgfgIntense.end());

        // Populates model with new data (colors, in this case)
        colorsModel_->setImgColors(dominantColors);
        colorsModel_->setBGFGColors(bgfg);

    } catch (TermColorException &e) {
        throw;
    }
}

void MainWindow::MainWindowImpl::doTerminals() {
    const auto &terminals = terminalsModel_->getTerminals();
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

void MainWindow::MainWindowImpl::onProcessColors(std::string_view imgPath) {
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

void MainWindow::MainWindowImpl::onProcessSave(std::string_view saveOption) {
    try {
        const auto termType = TerminalsModel::terminalToEnum_[saveOption.data()];

        // Construct appropriate terminal writer
        const auto writer = [&]() -> std::unique_ptr<Writer> {
            switch (termType) {
                case TerminalsModel::TerminalType::Konsole :
                    return std::make_unique<KonsoleWriter>();
                case TerminalsModel::TerminalType::Xfce4Terminal :
                    return std::make_unique<Xfce4TerminalWriter>();
                case TerminalsModel::TerminalType::LXTerminal :
                    std::make_unique<LXTerminalWriter>();
            }
            return nullptr;
        };

        const auto &colors = colorsModel_->getColors();
        writer()->writeToLocation(currentTimestamp(), colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);

    } catch (TermColorException &e) {
        throw;
    }
}

void MainWindow::MainWindowImpl::onSaveToJson(std::string_view saveLocation) {
    const auto jsonWriter = std::make_unique<JsonWriter>();
    const auto &colors = colorsModel_->getColors();
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

// MainWindow

MainWindow::MainWindow(int, char *[], QWidget *parent) : QMainWindow{parent} {
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
    setFixedSize(845, 500);
}

}

#include "MainWindow.moc"
