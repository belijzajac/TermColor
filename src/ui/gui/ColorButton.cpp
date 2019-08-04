#include "ColorButton.h"

#include <QPushButton>
#include <QLabel>
#include <QToolTip>
#include <QVBoxLayout>

ColorButton::ColorButton(const std::string &name, const color &color, QWidget *parent) : QWidget{parent}, name_{name},
                                                                                         color_{color} {
    setup();
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

    // label
    auto label = new QLabel{QString::fromStdString(name_), this};

    connect(btn_, SIGNAL(clicked()), this, SLOT(onClicked()));

    // adding everything together
    auto layout = new QVBoxLayout{this};
    layout->addWidget(btn_);
    layout->addWidget(label);
}


