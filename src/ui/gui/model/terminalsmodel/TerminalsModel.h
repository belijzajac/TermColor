#ifndef TERMINALSMODEL_H
#define TERMINALSMODEL_H

#include <QObject>
#include <memory>
#include <map>

class TerminalsModel : public QObject {
    class TerminalsModelImpl;
    Q_OBJECT
public:
    explicit TerminalsModel(QObject *parent = nullptr);
    ~TerminalsModel();

    // Terminal type
    enum class TerminalType {
        Konsole,
        GnomeTerminal
    };

    // Keys are strings and values are string-representing enums. The main purposes of this
    // is in creating the Factory design pattern for writing color-schemes to files
    static inline std::map<std::string, TerminalType> terminalToEnum_ {
        {"konsole",        TerminalType::Konsole},
        {"gnome-terminal", TerminalType::GnomeTerminal}
    };

    // To mark which application run was that: was it first or the following one onwards?
    enum class ChangedState { None, NewTerminals };

    // Storing terminals data
    struct Terminals {
        Terminals();

        ChangedState changedState_;

        std::vector<std::string> supported_; // names of supported terminal emulators
        std::vector<std::string> installed_; // installed terminal emulators in /bin
    };

    // Methods for Terminals
    const Terminals &getTerminals() const;
    void insertTerminals(const std::vector<std::string> &term);

signals:
    void modelChanged();

private:
    std::unique_ptr<TerminalsModelImpl> pimpl_;
};

#endif // TERMINALSMODEL_H
