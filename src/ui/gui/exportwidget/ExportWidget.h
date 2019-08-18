#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <QWidget>
#include <memory>

class GuiModel;

class ExportWidget : public QWidget {
    class ExportWidgetImpl;
    Q_OBJECT
public:
    explicit ExportWidget(const GuiModel &g, QWidget *parent = nullptr);
    ~ExportWidget();

private:
    std::unique_ptr<ExportWidgetImpl> pimpl_;
};

#endif // EXPORTWIDGET_H
