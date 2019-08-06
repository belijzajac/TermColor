#include "ColorsTableWidget.h"
#include "ColorButton.h"
#include <QGridLayout>

class ColorsTableWidget::ColorsTableWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ColorsTableWidgetImpl(ColorsTableWidget *parent);

private:
    void allocateButtons();

    //void makeConnections();
    void layoutButtons();

    ColorsTableWidget *parent_;
    QGridLayout *layout_;
    std::vector<ColorButton *> btns_;
};

ColorsTableWidget::ColorsTableWidgetImpl::ColorsTableWidgetImpl(ColorsTableWidget *parent) : QWidget{parent},
                                                                                             parent_{parent} {
    allocateButtons();

    layout_ = new QGridLayout{this};
    layoutButtons();
}

void ColorsTableWidget::ColorsTableWidgetImpl::allocateButtons() {
    // Background and foreground
    btns_.push_back(new ColorButton{"BG", {121, 129, 139}, this});
    btns_.push_back(new ColorButton{"FG", {197, 200, 198}, this});

    // Regular colors
    for (int i = 0; i <= 7; ++i)
        btns_.push_back(new ColorButton{std::to_string(i) + "_color", {255, 255, 255}, this});

    // Intense colors
    for (int i = 8; i <= 15; ++i)
        btns_.push_back(new ColorButton{std::to_string(i) + "_color", {240, 240, 240}, this});
}

void ColorsTableWidget::ColorsTableWidgetImpl::layoutButtons() {
    // Background and foreground
    layout_->addWidget(btns_.at(0), 0, 0);
    layout_->addWidget(btns_.at(1), 0, 1);

    int btnIndex = 2;
    // Regular colors
    for (int i = 2; i <= 9; ++i) {
        layout_->addWidget(btns_.at(btnIndex), i, 0);
        ++btnIndex;
    }

    // Intense colors
    for (int i = 2; i <= 9; ++i) {
        layout_->addWidget(btns_.at(btnIndex), i, 1);
        ++btnIndex;
    }
}

// ColorsTableWidget

ColorsTableWidget::ColorsTableWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ColorsTableWidgetImpl>(this);
}

ColorsTableWidget::~ColorsTableWidget() = default;

#include "ColorsTableWidget.moc"