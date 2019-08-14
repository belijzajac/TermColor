#ifndef COLORSTABLEWIDGET_H
#define COLORSTABLEWIDGET_H

#include <QWidget>
#include <memory>

class GuiModel;

class ColorsTableWidget : public QWidget {
    class ColorsTableWidgetImpl;
    Q_OBJECT
public:
    explicit ColorsTableWidget(const GuiModel &g, QWidget *parent = nullptr);
    ~ColorsTableWidget();

public slots:
    // call this slot when the model is populated with new colors
    void onModelChanged();

private:
    std::unique_ptr<ColorsTableWidgetImpl> pimpl_;
};

#endif // COLORSTABLEWIDGET_H
