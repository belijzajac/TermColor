#include "TerminalsModel.h"

namespace TermColor {

class TerminalsModel::TerminalsModelImpl {
public:
    explicit TerminalsModelImpl(TerminalsModel &p);

    // TerminalsModel::Terminals getters/setters
    const TerminalsModel::Terminals &getTerminals() const { return terminals_; };
    void insertTerminals(const std::vector<std::string> &term);

private:
    TerminalsModel &parent_;
    TerminalsModel::Terminals terminals_;
};

TerminalsModel::TerminalsModelImpl::TerminalsModelImpl(TerminalsModel &p) : parent_{p}, terminals_{} {}

void TerminalsModel::TerminalsModelImpl::insertTerminals(const std::vector<std::string> &term) {
    auto &terminals = terminals_.installed_;

    // First application run
    if(terminals_.changedState_ == ChangedState::None) {
        // Append newly found terminals
        for(const auto &t : term) {
            if (terminals.empty() || std::find(terminals.begin(), terminals.end(), t) == terminals.end())
                terminals.push_back(t);
        }
    }

    // Change state
    terminals_.changedState_ = ChangedState::NewTerminals;
    emit parent_.modelChanged();
}

// TerminalsModel

TerminalsModel::TerminalsModel(QObject *parent) : QObject{parent} {
    pimpl_ = std::make_unique<TerminalsModelImpl>(*this);
}

TerminalsModel::~TerminalsModel() = default;

TerminalsModel::Terminals::Terminals() : changedState_{ChangedState::None} {
    // Extract supported terminal emulators from terminalToEnum_
    for (const auto &[key, val] : terminalToEnum_)
        supported_.push_back(key);
}

const TerminalsModel::Terminals &TerminalsModel::getTerminals() const {
    return pimpl_->getTerminals();
}

void TerminalsModel::insertTerminals(const std::vector<std::string> &term) {
    pimpl_->insertTerminals(term);
}

}
