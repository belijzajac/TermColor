#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <memory>

class GuiModel : public QObject {
    class GuiModelImpl;
    Q_OBJECT
public:
    explicit GuiModel(QObject *parent = nullptr);
    ~GuiModel();

private:
    std::unique_ptr<GuiModelImpl> pimpl_;
};

#endif // GUIMODEL_H
