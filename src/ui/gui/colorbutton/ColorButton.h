#ifndef COLORSBUTTON_H
#define COLORSBUTTON_H

#include <backend/dominantcolor/DominantColor.h>
#include <QWidget>

class QPushButton;

class ColorButton : public QWidget {
    Q_OBJECT
public:
    ColorButton(const std::string &name, const color &color, QWidget *parent = nullptr);

    // Hovering over a button will the text to be displayed
    void registerToolTip();

    // Set button's stylesheet to match its color
    void setStylesheet();

    // Set button's color
    void setColor(const color &color);

private slots:
    void onClicked();

signals:
    void clicked(std::string color);

private:
    void setup();

    std::string name_;
    color color_;
    QPushButton *btn_;
};

#endif // COLORSBUTTON_H
