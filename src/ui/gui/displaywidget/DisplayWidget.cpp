#include "DisplayWidget.h"
#include <ui/gui/guimodel/GuiModel.h>
#include <QVBoxLayout>
#include <QTextEdit>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <QLabel>

class DisplayWidget::DisplayWidgetImpl : public QWidget {
public:
    DisplayWidgetImpl(const GuiModel &g, DisplayWidget *parent, QVBoxLayout *layout);
    void onModelChanged();

private:
    void initTextPieces();
    void doTextDspl();
    void setDsplLook(const color &color);

private:
    const GuiModel &guiModel_;
    QTextEdit *textDspl_;

    // Possible combinations of colors
    std::unordered_map<std::string, color> colorCombos_;          // e.g. {"regular_1", {255,255,255}}

    // Holding each text piece (text + its color)
    std::vector<std::tuple<std::string, std::string>> textPiece_; // e.g. {"#ifndef", "regular_1"}
};

DisplayWidget::DisplayWidgetImpl::DisplayWidgetImpl(const GuiModel &g, DisplayWidget *parent, QVBoxLayout *layout)
        : QWidget{parent}, guiModel_{g} {
    auto label = new QLabel{this};
    label->setText("Terminal preview");

    initTextPieces();

    textDspl_ = new QTextEdit{this};
    textDspl_->setFixedSize(600, 380);
    textDspl_->setReadOnly(true);

    layout->addWidget(label);
    layout->addWidget(textDspl_);
}

// Hard-coded a simple C++14 Exception class for demonstration purposes.
// If I was to read a C++ source file and dynamically change the colors' of a text,
// I'd have used the builder design pattern together with predefined colors for each C++ reserved keyword
void DisplayWidget::DisplayWidgetImpl::initTextPieces() {
    // Headers
    textPiece_.emplace_back("#ifndef EXCEPTION_H \n#define ", "regular_6");
    textPiece_.emplace_back("EXCEPTION_H \n\n", "regular_3");

    // Include
    textPiece_.emplace_back("#include ", "regular_6");
    textPiece_.emplace_back("<bits/exception.h> \n", "regular_2");

    // Include
    textPiece_.emplace_back("#include ", "regular_6");
    textPiece_.emplace_back("<string> \n\n", "regular_2");

    // Class definition
    textPiece_.emplace_back("class ", "regular_1");
    textPiece_.emplace_back("Exception : ", "regular_7");
    textPiece_.emplace_back("public ", "regular_1");
    textPiece_.emplace_back("std::exception { \n", "regular_7");

    // Class begin
    textPiece_.emplace_back("public ", "regular_1");
    textPiece_.emplace_back(":\n", "regular_5");

    // Class function
    textPiece_.emplace_back("   explicit ", "regular_1");
    textPiece_.emplace_back("Exception(", "regular_7");
    textPiece_.emplace_back("const ", "regular_1");
    textPiece_.emplace_back("std::string &msg) : msg_{msg} {} \n", "regular_7");

    // Another class function
    textPiece_.emplace_back("   const char ", "regular_1");
    textPiece_.emplace_back("*what() ", "regular_7");
    textPiece_.emplace_back("const ", "regular_1");
    textPiece_.emplace_back("noexcept ", "regular_7");
    textPiece_.emplace_back("override ", "regular_1");
    textPiece_.emplace_back("{ ", "regular_7");
    textPiece_.emplace_back("return ", "regular_5");
    textPiece_.emplace_back("msg_.c_str(); } \n\n", "regular_7");

    // Class members
    textPiece_.emplace_back("private ", "regular_1");
    textPiece_.emplace_back(":\n", "regular_5");
    textPiece_.emplace_back("   std::string msg_; \n}; \n\n", "regular_7");

    // End of class
    textPiece_.emplace_back("#endif ", "regular_6");
    textPiece_.emplace_back("// EXCEPTION_H", "regular_4");
}

void DisplayWidget::DisplayWidgetImpl::doTextDspl() {
    // Save previous text info
    const auto oldTextColor = textDspl_->textColor();

    std::for_each(textPiece_.begin(), textPiece_.end(), [&oldTextColor, this](const auto &t) {
        // Extract color representing key
        const auto &textColorStr = std::get<1>(t);

        // Lookup exact color using they key
        const auto &exactColor = colorCombos_[textColorStr];

        // Append text sub-string with the new color
        textDspl_->setTextColor(QColor::fromRgb(exactColor.r, exactColor.g, exactColor.b));
        textDspl_->insertPlainText(QString::fromStdString(std::get<0>(t)));

        // Restore old properties
        textDspl_->setTextColor(oldTextColor);
    });
}

void DisplayWidget::DisplayWidgetImpl::setDsplLook(const color &color) {
    auto palette = textDspl_->palette();
    palette.setColor(QPalette::Base, QColor::fromRgb(color.r, color.g, color.b));
    textDspl_->setPalette(palette);

    // Increase font point size
    auto font = textDspl_->font();
    const auto pointSize = font.pointSize();
    font.setPointSize(pointSize + 2);

    textDspl_->setFont(font);
    textDspl_->update();
}

void DisplayWidget::DisplayWidgetImpl::onModelChanged() {
    const GuiModel::Colors &colors = guiModel_.getColors();

    // Populate colorCombos_
    for (unsigned long i = 0; i < colors.regular_.size(); ++i)
        colorCombos_["regular_" + std::to_string(i)] = colors.regular_.at(i);

    // Set textDspl_'s colors
    doTextDspl();
    setDsplLook(colors.BGFG_.at(0));
}

// DisplayWidget

DisplayWidget::DisplayWidget(const GuiModel &g, QWidget *parent) {
    auto layout = new QVBoxLayout{this};
    pimpl_ = new DisplayWidgetImpl{g, this, layout};
    layout->addWidget(pimpl_);
}

void DisplayWidget::onModelChanged() {
    pimpl_->onModelChanged();
}
