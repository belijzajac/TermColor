#include "ColorsTableWidget.h"

#include <ui/gui/widget/colorbutton/ColorButton.h>
#include <ui/gui/model/colorsmodel/ColorsModel.h>
#include <QGridLayout>
#include <QLabel>

namespace TermColor {

using namespace TermColor::Utils;

class ColorsTableWidget::ColorsTableWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ColorsTableWidgetImpl(const ColorsModel &c, ColorsTableWidget *parent);
    QVBoxLayout *getLayout() { return verticalLayout_; }
    void onModelChanged();

private:
    void allocateButtons();
    void layoutButtons();
    void layoutOtherButtons(const std::vector<ColorButton *> &btns, int column);
    void updateBtnsColors(const std::vector<ColorButton *> &btns, const std::vector<color> &colors);

    const ColorsModel &colorsModel_;
    ColorsTableWidget *parent_;

    // Layouts
    QVBoxLayout *verticalLayout_;
    QGridLayout *gridLayout_;

    // Color buttons
    std::vector<ColorButton *> bgfgBtns_;      // Background and Foreground (2)
    std::vector<ColorButton *> regularBtns_;   // Buttons of regular colors (8)
    std::vector<ColorButton *> intenseBtns_;   // Buttons of intense colors (8)
};

ColorsTableWidget::ColorsTableWidgetImpl::ColorsTableWidgetImpl(const ColorsModel &c, ColorsTableWidget *parent)
    : QWidget{parent}
    , parent_{parent}
    , verticalLayout_{new QVBoxLayout{this}}
    , gridLayout_{new QGridLayout}
    , colorsModel_{c}
{
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
    std::for_each(btns.begin(), btns.end(), [this, row = 2, column](ColorButton *btn) mutable {
        gridLayout_->addWidget(btn, row, column);
        ++row;
    });
}

void ColorsTableWidget::ColorsTableWidgetImpl::onModelChanged() {
    const auto &colors = colorsModel_.getColors();
    const auto state = colors.changedState_;

    if (state == ColorsModel::ChangedState::NewColors) {
        // Regular colors and Intense colors
        updateBtnsColors(regularBtns_, colors.regular_);
        updateBtnsColors(intenseBtns_, colors.intense_);

    } else if (state == ColorsModel::ChangedState::Background) {
        // Background and Foreground
        updateBtnsColors(bgfgBtns_, colors.BGFG_);
    } else {
        // Do nothing
    }
}

void ColorsTableWidget::ColorsTableWidgetImpl::updateBtnsColors(const std::vector<ColorButton *> &btns,
                                                                const std::vector<color> &colors) {
    for (unsigned long btnInd = 0; btnInd < btns.size(); ++btnInd) {
        btns.at(btnInd)->setColor(colors.at(btnInd));

        // In case we've managed to obtain less colors than there are actual buttons
        if (btnInd >= colors.size()) {
            continue;
        }
    }
}

// ColorsTableWidget

ColorsTableWidget::ColorsTableWidget(const ColorsModel &c, QWidget * /*parent*/) {
    pimpl_ = std::make_unique<ColorsTableWidgetImpl>(c, this);
    this->setLayout(pimpl_->getLayout());
}

void ColorsTableWidget::onModelChanged() {
    pimpl_->onModelChanged();
}

ColorsTableWidget::~ColorsTableWidget() = default;

}

#include "ColorsTableWidget.moc"
