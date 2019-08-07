#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

class QTextEdit;

class DisplayWidget : public QWidget {
    class DisplayWidgetImpl;
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = nullptr);

private:
    DisplayWidgetImpl *pimpl_;
};

#endif // DISPLAYWIDGET_H
