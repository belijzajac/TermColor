#ifndef COLORSTABLEWIDGET_H
#define COLORSTABLEWIDGET_H

#include <QWidget>
#include <memory>

class QGridLayout;

class ColorsTableWidget : public QWidget {
    class ColorsTableWidgetImpl;
    Q_OBJECT
public:
    explicit ColorsTableWidget(QWidget *parent = nullptr);
    ~ColorsTableWidget();

private:
    std::unique_ptr<ColorsTableWidgetImpl> pimpl_;
};

#endif // COLORSTABLEWIDGET_H
