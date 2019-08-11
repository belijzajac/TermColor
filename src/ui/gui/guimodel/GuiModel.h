#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <memory>
#include <backend/dominantcolor/DominantColor.h>

class GuiModel : public QObject {
    class GuiModelImpl;
    Q_OBJECT
public:
    explicit GuiModel(QObject *parent = nullptr);
    ~GuiModel();

    // Storing colors data
    struct Colors {
        Colors();

        std::vector<color> BGFG_;     // Background and Foreground colors
        std::vector<color> regular_;  // Regular colors
        std::vector<color> intense_;  // Intense colors
    };

    const Colors &getColors() const;

public slots:
    void onImageDropped(const QString& path);

signals:
    void hideImageDropWidget();

private:
    std::unique_ptr<GuiModelImpl> pimpl_;
};

#endif // GUIMODEL_H
