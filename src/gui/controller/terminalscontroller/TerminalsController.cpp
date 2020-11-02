#include "TerminalsController.h"

#include "backend/writer/Writer.h"
#include "backend/writer/konsolewriter/KonsoleWriter.h"
#include "backend/writer/xfce4terminalwriter/Xfce4TerminalWriter.h"
#include "backend/writer/lxterminalwriter/LXTerminalWriter.h"
#include "backend/writer/jsonwriter/JsonWriter.h"
#include "backend/exception/Exception.h"

#if defined(__GNUC__) && !__has_include(<filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include <ctime>

namespace TermColor {

TerminalsController::TerminalsController(TerminalsModel *t, const ColorsModel &c, QObject *parent)
    : QObject(parent)
    , terminalsModel_{t}
    , colorsModel_{c}
{
    //...
}

void TerminalsController::doTerminals() {
    const auto &terminals = terminalsModel_->getTerminals();
    auto supportedTerminals = terminals.supported_;
    std::vector<std::string> terminalsFound; // Found terminals in /bin

    // Find supported terminals in /bin
    for (const auto &dirEntry : fs::recursive_directory_iterator("/bin")) {
        const auto strEntry = dirEntry.path().string(); // e.g. "/bin/konsole"

        // Loop through terminals
        for (auto it = supportedTerminals.begin(); it != supportedTerminals.end();) {
            // We compare the length in case of an entry such as "/bin/konsoleprofile" if we
            // only need "/bin/konsole". Then we compare its substring against supported terminals
            if (strEntry.length() - 5 == it->length() && strEntry.substr(5, it->length()) == *it) {
                // Append list of found terminals and remove already found entry
                terminalsFound.push_back(*it);
                it = supportedTerminals.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Check if there are any found terminals
    if(terminalsFound.empty()) {
        emit noTerminalsFound();
    } else {
        // Populate model with new terminals
        terminalsModel_->insertTerminals(terminalsFound);
    }
}

const std::string currentTimestamp() {
    std::time_t  time = std::time(nullptr);
    std::tm *timeNow  = std::localtime(&time);

    return std::to_string(timeNow->tm_year + 1900) + '-' +
           std::to_string(timeNow->tm_mon + 1) + '-' +
           std::to_string(timeNow->tm_mday) + "-" +
           std::to_string(timeNow->tm_hour) + "-" +
           std::to_string(timeNow->tm_min) + "-" +
           std::to_string(timeNow->tm_sec);
}

void TerminalsController::onProcessSave(std::string_view saveOption) {
    using namespace TermColor::Utils;

    try {
        const auto termType = TerminalsModel::terminalToEnum_[saveOption.data()];

        // Construct appropriate terminal writer
        const auto writer = [&]() -> std::unique_ptr<Writer> {
            switch (termType) {
                case TerminalsModel::TerminalType::Konsole :
                    return std::make_unique<KonsoleWriter>();
                case TerminalsModel::TerminalType::Xfce4Terminal :
                    return std::make_unique<Xfce4TerminalWriter>();
                case TerminalsModel::TerminalType::LXTerminal :
                    std::make_unique<LXTerminalWriter>();
            }
            return nullptr;
        };

        const auto &colors = colorsModel_.getColors();
        writer()->writeToLocation(currentTimestamp(), colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);

    } catch (TermColorException &e) {
        throw;
    }
}

void TerminalsController::onSaveToJson(std::string_view saveLocation) {
    const auto jsonWriter = std::make_unique<JsonWriter>();
    const auto &colors = colorsModel_.getColors();
    jsonWriter->writeToLocation(saveLocation, colors.BGFG_, colors.BGFGintense_, colors.regular_, colors.intense_);
}

}
