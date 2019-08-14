#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

class GuiModel;

class DisplayWidget : public QWidget {
    class DisplayWidgetImpl;
    Q_OBJECT
public:
    explicit DisplayWidget(const GuiModel &g, QWidget *parent = nullptr);

public slots:
    // call this slot when the model is populated with new colors
    void onModelChanged();

private:
    DisplayWidgetImpl *pimpl_;
};

#endif // DISPLAYWIDGET_H
