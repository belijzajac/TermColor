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

    // Methods for Colors
    const Colors &getColors() const;
    void setColors(const std::vector<color> &colors);

    // Storing terminals data
    struct Terminals {
        Terminals();

        std::vector<std::string> supported_; // names of supported terminal emulators
        std::vector<std::string> installed_; // installed terminal emulators in /bin
    };

    // Methods for Terminals
    const Terminals &getTerminals() const;
    void insertTerminal(const std::string &term);

public slots:
    void onImageDropped(const QString& path);

signals:
    void hideImageDropWidget();
    void doProcessColors(const std::string &);
    void modelChanged();

private:
    std::unique_ptr<GuiModelImpl> pimpl_;
};

#endif // GUIMODEL_H
