#include "ColorButton.h"
#include <QPushButton>
#include <QVBoxLayout>

namespace TermColor {

using namespace TermColor::Utils;

ColorButton::ColorButton(const std::string &name, const color &color, QWidget *parent)
    : QWidget{parent}
    , name_{name}
    , color_{color}
{
    setup();
}

void ColorButton::registerToolTip() {
    const QString tooltip = QString{"(%1, %2, %3)"}.arg(color_.r).arg(color_.g).arg(color_.b);
    btn_->setToolTip(tooltip);
}

void ColorButton::setStylesheet() {
    btn_->setFlat(true);
    btn_->setAutoFillBackground(true);
    setColor(this->color_);
}

void ColorButton::setColor(const color &color) {
    color_ = color;
    registerToolTip();

    QPalette pal = btn_->palette();
    pal.setColor(QPalette::Button, QColor(QColor::fromRgb(color_.r, color_.g, color_.b)));
    btn_->setPalette(pal);
    btn_->update();
}

void ColorButton::setup() {
    // button
    btn_ = new QPushButton{QString::fromStdString(name_), this};
    setStylesheet();

    // adding everything together
    auto layout = new QVBoxLayout{this};
    layout->addWidget(btn_);
}

}
