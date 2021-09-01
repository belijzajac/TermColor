#ifndef COLORSBUTTON_H
#define COLORSBUTTON_H

#include "backend/dominantcolor/DominantColor.h"
#include <QWidget>

class QPushButton;

namespace TermColor {

class ColorButton : public QWidget {
    Q_OBJECT
public:
    ColorButton(std::string_view name, const color_t &color, QWidget *parent = nullptr);

    // Hovering over a button will the text to be displayed
    void registerToolTip();

    // Set button's stylesheet to match its color
    void setStylesheet();

    // Set button's color
    void setColor(const color_t &color);

private:
    void setup();

    std::string name_;
    color_t color_;
    QPushButton *btn_;
};

}

#endif // COLORSBUTTON_H
