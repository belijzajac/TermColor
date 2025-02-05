#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <QWidget>
#include <memory>

namespace TermColor {

class TerminalsModel;

class ExportWidget : public QWidget {
    class ExportWidgetImpl;
    Q_OBJECT
public:
    explicit ExportWidget(const TerminalsModel &t, QWidget *parent = nullptr);
    ~ExportWidget();

    // Disable all the means of saving color schemes
    void disableSaveBtn();

public slots:
    // call this slot when the model has terminal emulators data updated
    void onModelChanged();
    void onSaveBtnClicked();

signals:
    void saveBtnClicked(std::string_view option);
    void saveToJsonBtnClicked(std::string_view fileLocation);

private:
    std::unique_ptr<ExportWidgetImpl> pimpl_;
};

}

#endif // EXPORTWIDGET_H
