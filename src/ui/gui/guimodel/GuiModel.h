#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <memory>
#include <backend/dominantcolor/DominantColor.h>
#include <map>

enum class TerminalType {
    Konsole,
    GnomeTerminal
};

class GuiModel : public QObject {
    class GuiModelImpl;
    Q_OBJECT
public:
    explicit GuiModel(QObject *parent = nullptr);
    ~GuiModel();

    //-------------------------------------- Colors --------------------------------------
    // Storing colors data
    struct Colors {
        Colors();

        std::vector<color> BGFG_;        // Background and Foreground colors
        std::vector<color> BGFGintense_; // Same as above, but intense
        std::vector<color> regular_;     // Regular colors
        std::vector<color> intense_;     // Intense colors
    };

    // Methods for Colors
    const Colors &getColors() const;
    void setImgColors(const std::vector<color> &colors);
    void setBGFGColors(const std::vector<color> &bgfg);

    //-------------------------------------- Terminals --------------------------------------
    // Keys are strings and values are string-representing enums. The main purposes of this
    // is in creating the Factory design pattern for writing color-schemes to files
    static inline std::map<std::string, TerminalType> terminalToEnum_ {
        {"konsole",        TerminalType::Konsole},
        {"gnome-terminal", TerminalType::GnomeTerminal}
    };

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
    void bgfgColorChanged();

private:
    std::unique_ptr<GuiModelImpl> pimpl_;
};

#endif // GUIMODEL_H
