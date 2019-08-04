#ifndef COLORSBUTTON_H
#define COLORSBUTTON_H

#include <backend/dominantcolor/DominantColor.h>
#include <QWidget>

class QPushButton;

class ColorButton : public QWidget {
    Q_OBJECT
public:
    ColorButton(const std::string &name, const color &color, QWidget *parent = nullptr);
    
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
