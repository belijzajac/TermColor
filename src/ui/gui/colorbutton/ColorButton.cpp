#include "ColorButton.h"

#include <QPushButton>
#include <QToolTip>
#include <QVBoxLayout>

ColorButton::ColorButton(const std::string &name, const color &color, QWidget *parent) : QWidget{parent}, name_{name},
                                                                                         color_{color} {
    setup();
}

void ColorButton::registerToolTip() {
    const QString tooltip = QString{"(%1, %2, %3)"}.arg(color_.r).arg(color_.g).arg(color_.b);
    btn_->setToolTip(tooltip);
}

void ColorButton::setStylesheet() {
    btn_->setFlat(true);

    QPalette pal = btn_->palette();
    pal.setColor(QPalette::Button, QColor(QColor::fromRgb(color_.r, color_.g, color_.b)));
    btn_->setAutoFillBackground(true);
    btn_->setPalette(pal);
    btn_->update();
}

void ColorButton::onClicked() {
    std::string dsplText{"(" + std::to_string(color_.r) + ", "
                         + std::to_string(color_.g) + ", "
                         + std::to_string(color_.b) + ")"};

    emit clicked(dsplText);
}


void ColorButton::setup() {
    // button
    btn_ = new QPushButton{QString::fromStdString(name_), this};
    registerToolTip();
    setStylesheet();

    // connect button's signals and slots
    connect(btn_, SIGNAL(clicked()), this, SLOT(onClicked()));

    // adding everything together
    auto layout = new QVBoxLayout{this};
    layout->addWidget(btn_);
}
