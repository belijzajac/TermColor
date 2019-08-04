#include "ColorsTableWidget.h"
#include "ColorButton.h"
#include <QGridLayout>

class ColorsTableWidget::ColorsTableWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ColorsTableWidgetImpl(ColorsTableWidget *parent);

private:
    void allocateButtons();
    void layoutButtons();

    ColorsTableWidget *parent_;
    QGridLayout *layout_;
    std::vector<ColorButton*> btns_;
};

ColorsTableWidget::ColorsTableWidgetImpl::ColorsTableWidgetImpl(ColorsTableWidget *parent) : QWidget{parent},
                                                                                             parent_{parent} {
    allocateButtons();

    layout_ = new QGridLayout;
    layoutButtons();
}

void ColorsTableWidget::ColorsTableWidgetImpl::allocateButtons() {
    for(int i = 0; i<=7; ++i){
        btns_.push_back(new ColorButton{std::to_string(i) + "_color", {255,255,255}, this});
    }
}

void ColorsTableWidget::ColorsTableWidgetImpl::layoutButtons() {
    layout_->addWidget(btns_.at(0), 0, 0);
    layout_->addWidget(btns_.at(1), 0, 1);
    layout_->addWidget(btns_.at(2), 1, 0);
    layout_->addWidget(btns_.at(3), 1, 1);
    layout_->addWidget(btns_.at(4), 2, 0);
    layout_->addWidget(btns_.at(5), 2, 1);
    layout_->addWidget(btns_.at(6), 3, 0);
    layout_->addWidget(btns_.at(7), 3, 1);
}

// ColorsTableWidget

ColorsTableWidget::ColorsTableWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ColorsTableWidgetImpl>(this);
}

ColorsTableWidget::~ColorsTableWidget() = default;

#include "ColorsTableWidget.moc"