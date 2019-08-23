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
    void onbgfgColorChanged();

private:
    void initTextPieces();
    void doTextDspl();
    void setDsplLook(const color &color);
    void setFont();

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
    textDspl_->setFixedSize(600, 415);
    textDspl_->setReadOnly(true);

    setFont();

    layout->addWidget(label);
    layout->addWidget(textDspl_);
}

// Hard-coded a simple C++14 Exception class for demonstration purposes.
// If I was to read a C++ source file and dynamically change the colors' of a text,
// I'd have used the builder design pattern together with predefined colors for each C++ reserved keyword
void DisplayWidget::DisplayWidgetImpl::initTextPieces() {
    // Headers
    textPiece_.emplace_back("#ifndef EXCEPTION_H \n#define ", "intense_6");
    textPiece_.emplace_back("EXCEPTION_H \n\n", "intense_1");

    // Include
    textPiece_.emplace_back("#include ", "intense_6");
    textPiece_.emplace_back("<bits/exception.h> \n", "intense_3");

    // Include
    textPiece_.emplace_back("#include ", "intense_6");
    textPiece_.emplace_back("<string> \n\n", "intense_3");

    // Class definition
    textPiece_.emplace_back("class ", "regular_2");
    textPiece_.emplace_back("Exception : ", "foreground");
    textPiece_.emplace_back("public ", "regular_2");
    textPiece_.emplace_back("std::exception { \n", "foreground");

    // Class begin
    textPiece_.emplace_back("public ", "regular_2");
    textPiece_.emplace_back(":\n", "regular_5");

    // Class function
    textPiece_.emplace_back("   explicit ", "regular_2");
    textPiece_.emplace_back("Exception(", "foreground");
    textPiece_.emplace_back("const ", "regular_2");
    textPiece_.emplace_back("std::string &msg) : msg_{msg} {} \n", "foreground");

    // Another class function
    textPiece_.emplace_back("   const char ", "regular_2");
    textPiece_.emplace_back("*what() ", "foreground");
    textPiece_.emplace_back("const ", "regular_2");
    textPiece_.emplace_back("noexcept ", "foreground");
    textPiece_.emplace_back("override ", "regular_2");
    textPiece_.emplace_back("{ ", "foreground");
    textPiece_.emplace_back("return ", "regular_5");
    textPiece_.emplace_back("msg_.c_str(); } \n\n", "foreground");

    // Class members
    textPiece_.emplace_back("private ", "regular_2");
    textPiece_.emplace_back(":\n", "regular_5");
    textPiece_.emplace_back("   std::string msg_; \n}; \n\n", "foreground");

    // End of class
    textPiece_.emplace_back("#endif ", "intense_6");
    textPiece_.emplace_back("// EXCEPTION_H", "intense_4");
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

    textDspl_->update();
}

void DisplayWidget::DisplayWidgetImpl::setFont() {
    // Increase font point size
    auto font = textDspl_->font();
    const auto pointSize = font.pointSize();
    font.setPointSize(pointSize + 2);

    textDspl_->setFont(font);
    textDspl_->update();
}

void DisplayWidget::DisplayWidgetImpl::onModelChanged() {
    const GuiModel::Colors &colors = guiModel_.getColors();

    // Populate colorCombos_ with regular colors
    for (unsigned long i = 0; i < colors.regular_.size(); ++i)
        colorCombos_["regular_" + std::to_string(i)] = colors.regular_.at(i);

    // Populate colorCombos_ with intense colors
    for (unsigned long i = 0; i < colors.intense_.size(); ++i)
        colorCombos_["intense_" + std::to_string(i)] = colors.intense_.at(i);

    // Populate colorCombos_ with foreground color
    colorCombos_["foreground"] = colors.BGFG_.at(1);

    // Set textDspl_'s colors
    doTextDspl();
    setDsplLook(colors.BGFG_.at(0));
}

void DisplayWidget::DisplayWidgetImpl::onbgfgColorChanged() {
    const GuiModel::Colors &colors = guiModel_.getColors();
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

void DisplayWidget::onbgfgColorChanged() {
    pimpl_->onbgfgColorChanged();
}
