#include "ColorsTableWidget.h"
#include <ui/gui/colorbutton/ColorButton.h>
#include <QGridLayout>
#include <QLabel>

class ColorsTableWidget::ColorsTableWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ColorsTableWidgetImpl(ColorsTableWidget *parent);
    QVBoxLayout *getLayout() { return verticalLayout_; }

private:
    void allocateButtons();
    void layoutButtons();

    //void makeConnections();

    ColorsTableWidget *parent_;
    QVBoxLayout *verticalLayout_;
    QGridLayout *gridLayout_;
    std::vector<ColorButton *> btns_;
};

ColorsTableWidget::ColorsTableWidgetImpl::ColorsTableWidgetImpl(ColorsTableWidget *parent)
        : QWidget{parent}, parent_{parent}, verticalLayout_{new QVBoxLayout{this}}, gridLayout_{new QGridLayout} {

    auto label = new QLabel{this};
    label->setText("Terminal colors");

    verticalLayout_->addWidget(label);
    verticalLayout_->addLayout(gridLayout_);

    allocateButtons();
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
    gridLayout_->addWidget(btns_.at(0), 0, 0);
    gridLayout_->addWidget(btns_.at(1), 0, 1);

    unsigned long btnIndex = 2;
    // Regular colors
    for (int i = 2; i <= 9; ++i) {
        gridLayout_->addWidget(btns_.at(btnIndex), i, 0);
        ++btnIndex;
    }

    // Intense colors
    for (int i = 2; i <= 9; ++i) {
        gridLayout_->addWidget(btns_.at(btnIndex), i, 1);
        ++btnIndex;
    }
}

// ColorsTableWidget

ColorsTableWidget::ColorsTableWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ColorsTableWidgetImpl>(this);
    this->setLayout(pimpl_->getLayout());
}

ColorsTableWidget::~ColorsTableWidget() = default;

#include "ColorsTableWidget.moc"