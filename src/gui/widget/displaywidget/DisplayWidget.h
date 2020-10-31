#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

namespace TermColor {

class ColorsModel;

class DisplayWidget : public QWidget {
    class DisplayWidgetImpl;
    Q_OBJECT
public:
    explicit DisplayWidget(const ColorsModel &c, QWidget *parent = nullptr);

public slots:
    // call this slot when the model is populated with new colors
    void onModelChanged();

private:
    DisplayWidgetImpl *pimpl_;
};

}

#endif // DISPLAYWIDGET_H
