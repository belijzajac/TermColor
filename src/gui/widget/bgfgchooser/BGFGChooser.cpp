#include "BGFGChooser.h"

#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>

namespace TermColor {

BGFGChooser::BGFGChooser(QWidget *parent) : QWidget{parent} {
    // Initialize buttons and a label
    btnGroup_ = new QButtonGroup{this};
    darkBtn_  = new QRadioButton{"Dark", this};
    darkBtn_->setChecked(true);
    lightBtn_ = new QRadioButton{"Light", this};
    auto label = new QLabel{"BG/FG:", this};

    // adding everything together
    auto hLayout = new QHBoxLayout{this};
    hLayout->addWidget(label);
    hLayout->addWidget(darkBtn_);
    hLayout->addWidget(lightBtn_);

    btnGroup_->addButton(darkBtn_, 0);
    btnGroup_->addButton(lightBtn_, 1);

    doConnections();
}

void BGFGChooser::doConnections() {
    // darkBtn_ (id = 0) or lightBtn_ (id = 1) is clicked
    connect(btnGroup_, &QButtonGroup::idClicked, [=](int id) {
        if (id == 0 || id == 1) {
            emit radioBtnClicked(id);
        }
    });
}

}
