#include "ColorsTableWidget.h"
#include <ui/gui/colorbutton/ColorButton.h>
#include <ui/gui/guimodel/GuiModel.h>
#include <QGridLayout>
#include <QLabel>

class ColorsTableWidget::ColorsTableWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ColorsTableWidgetImpl(const GuiModel &g, ColorsTableWidget *parent);
    QVBoxLayout *getLayout() { return verticalLayout_; }
    void onModelChanged();

private:
    void allocateButtons();
    void layoutButtons();
    void layoutOtherButtons(const std::vector<ColorButton *> &btns, int column);
    void updateBtnsColors(const std::vector<ColorButton *> &btns, const std::vector<color> &colors);

    const GuiModel &guiModel_;
    ColorsTableWidget *parent_;

    // Layouts
    QVBoxLayout *verticalLayout_;
    QGridLayout *gridLayout_;

    // Color buttons
    std::vector<ColorButton *> bgfgBtns_;      // Background and Foreground (2)
    std::vector<ColorButton *> regularBtns_;   // Buttons of regular colors (8)
    std::vector<ColorButton *> intenseBtns_;   // Buttons of intense colors (8)
};

ColorsTableWidget::ColorsTableWidgetImpl::ColorsTableWidgetImpl(const GuiModel &g, ColorsTableWidget *parent)
        : QWidget{parent}, parent_{parent}, verticalLayout_{new QVBoxLayout{this}}, gridLayout_{new QGridLayout},
          guiModel_{g} {

    auto label = new QLabel{this};
    label->setText("Terminal colors");

    verticalLayout_->addWidget(label);
    verticalLayout_->addLayout(gridLayout_);

    allocateButtons();
    layoutButtons();
}

void ColorsTableWidget::ColorsTableWidgetImpl::allocateButtons() {
    // Background and foreground
    bgfgBtns_.push_back(new ColorButton{"BG", {121, 129, 139}, this});
    bgfgBtns_.push_back(new ColorButton{"FG", {197, 200, 198}, this});

    // Regular and intense colors
    for (int i = 0; i <= 7; ++i) {
        regularBtns_.push_back(new ColorButton{std::to_string(i) + "_color", {255, 255, 255}, this});
        intenseBtns_.push_back(new ColorButton{std::to_string(i) + "_color", {240, 240, 240}, this});
    }
}

void ColorsTableWidget::ColorsTableWidgetImpl::layoutButtons() {
    // Background and foreground
    gridLayout_->addWidget(bgfgBtns_.at(0), 0, 0);
    gridLayout_->addWidget(bgfgBtns_.at(1), 0, 1);

    // Regular colors
    layoutOtherButtons(regularBtns_, 0);

    // Intense colors
    layoutOtherButtons(intenseBtns_, 1);
}

void ColorsTableWidget::ColorsTableWidgetImpl::layoutOtherButtons(const std::vector<ColorButton *> &btns, int column) {
    int rowNum = 2;

    // Regular colors
    std::for_each(btns.begin(), btns.end(), [this, &rowNum, column](ColorButton *btn) {
        gridLayout_->addWidget(btn, rowNum, column);
        ++rowNum;
    });
}

void ColorsTableWidget::ColorsTableWidgetImpl::onModelChanged() {
    const GuiModel::Colors &colors = guiModel_.getColors();

    // Background and Foreground
    updateBtnsColors(bgfgBtns_, colors.BGFG_);

    // Regular colors
    updateBtnsColors(regularBtns_, colors.regular_);

    // Intense colors
    updateBtnsColors(intenseBtns_, colors.intense_);
}

void ColorsTableWidget::ColorsTableWidgetImpl::updateBtnsColors(const std::vector<ColorButton *> &btns,
                                                                const std::vector<color> &colors) {
    for (unsigned long btnInd = 0; btnInd < btns.size(); ++btnInd) {
        btns.at(btnInd)->setColor(colors.at(btnInd));

        // In case we've managed to obtain less colors than there are actual buttons
        if (btnInd >= colors.size())
            continue;
    }
}

// ColorsTableWidget

ColorsTableWidget::ColorsTableWidget(const GuiModel &g, QWidget *parent) {
    pimpl_ = std::make_unique<ColorsTableWidgetImpl>(g, this);
    this->setLayout(pimpl_->getLayout());
}

void ColorsTableWidget::onModelChanged() {
    pimpl_->onModelChanged();
}

ColorsTableWidget::~ColorsTableWidget() = default;

#include "ColorsTableWidget.moc"