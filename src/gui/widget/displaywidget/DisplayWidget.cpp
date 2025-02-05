#include "DisplayWidget.h"

#include "gui/model/colorsmodel/ColorsModel.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <QLabel>

namespace TermColor {

using namespace TermColor::Utils;

class DisplayWidget::DisplayWidgetImpl : public QWidget {
public:
    DisplayWidgetImpl(const ColorsModel &c, DisplayWidget *parent, QVBoxLayout *layout);
    void onModelChanged();

private:
    void initTextPieces();
    void doTextDspl();
    void setDsplLook(const color_t &color);
    void setFont();

private:
    const ColorsModel &colorsModel_;
    QTextEdit *textDspl_;

    // Possible combinations of colors
    std::unordered_map<std::string, color_t> colorCombos_;          // e.g. {"regular_1", {255,255,255}}

    // Holding each text piece (text + its color)
    std::vector<std::tuple<std::string, std::string>> textPiece_; // e.g. {"#ifndef", "regular_1"}
};

DisplayWidget::DisplayWidgetImpl::DisplayWidgetImpl(const ColorsModel &c, DisplayWidget *parent, QVBoxLayout *layout)
    : QWidget{parent}
    , colorsModel_{c}
{
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
void DisplayWidget::DisplayWidgetImpl::initTextPieces() {
    textPiece_ = { // Headers
                   {"#ifndef EXCEPTION_H \n#define ", "intense_6"},
                   {"EXCEPTION_H \n\n", "intense_1"},

                   // Include
                   {"#include ", "intense_6"},
                   {"<bits/exception.h> \n", "intense_3"},
                   {"#include ", "intense_6"},
                   {"<string> \n\n", "intense_3"},

                   // Class definition
                   {"class ", "regular_2"},
                   {"Exception : ", "foreground"},
                   {"public ", "regular_2"},
                   {"std::exception { \n", "foreground"},

                   // Class begin
                   {"public ", "regular_2"},
                   {":\n", "regular_5"},

                   // Class function
                   {"   explicit ", "regular_2"},
                   {"Exception(", "foreground"},
                   {"const ", "regular_2"},
                   {"std::string &msg) : msg_{msg} {} \n", "foreground"},

                   // Another class function
                   {"   const char ", "regular_2"},
                   {"*what() ", "foreground"},
                   {"const ", "regular_2"},
                   {"noexcept ", "foreground"},
                   {"override ", "regular_2"},
                   {"{ ", "foreground"},
                   {"return ", "regular_5"},
                   {"msg_.c_str(); } \n\n", "foreground"},

                   // Class members
                   {"private ", "regular_2"},
                   {":\n", "regular_5"},
                   {"   std::string msg_; \n}; \n\n", "foreground"},

                   // End of class
                   {"#endif ", "intense_6"},
                   {"// EXCEPTION_H", "intense_4"}
    };
}

void DisplayWidget::DisplayWidgetImpl::doTextDspl() {
    // Reset textDspl_
    textDspl_->clear();

    // Save previous text info
    const auto &oldTextColor = textDspl_->textColor();

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

void DisplayWidget::DisplayWidgetImpl::setDsplLook(const color_t &color) {
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
    const auto &colors = colorsModel_.getColors();
    const auto &state = colors.changedState_;

    if (state == ColorsModel::ChangedState::NewColors) {
        // Populate colorCombos_ with regular colors
        for (unsigned long i = 0; i < colors.regular_.size(); ++i) {
            colorCombos_["regular_" + std::to_string(i)] = colors.regular_.at(i);
        }
        // Populate colorCombos_ with intense colors
        for (unsigned long i = 0; i < colors.intense_.size(); ++i) {
            colorCombos_["intense_" + std::to_string(i)] = colors.intense_.at(i);
        }
    } else if (state == ColorsModel::ChangedState::Background) {
        // Populate colorCombos_ with foreground color
        // If the key already exists, just change the foreground
        colorCombos_.try_emplace("foreground", colors.BGFG_.at(1));
        // Only at this point of the state we have sufficient data for the textDspl_
        setDsplLook(colors.BGFG_.at(0));
        doTextDspl();
    } else {
        // Do nothing
    }
}

// DisplayWidget

DisplayWidget::DisplayWidget(const ColorsModel &c, QWidget * /*parent*/) {
    auto layout = new QVBoxLayout{this};
    pimpl_ = new DisplayWidgetImpl{c, this, layout};
    layout->addWidget(pimpl_);
}

void DisplayWidget::onModelChanged() {
    pimpl_->onModelChanged();
}

}
