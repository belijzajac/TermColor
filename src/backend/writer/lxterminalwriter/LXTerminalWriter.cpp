#include "LXTerminalWriter.h"

#include "backend/exception/Exception.h"
#include <fstream>

#if defined(__GNUC__) && !__has_include(<filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace TermColor {

using namespace TermColor::Utils;

LXTerminalWriter::LXTerminalWriter()
    : Writer(".config/lxterminal/"
    , {"lxterminal", ".conf"})
{

}

void LXTerminalWriter::writeToLocation(std::string_view name,
                                       const std::vector<color> &bgfg,
                                       const std::vector<color> &bgfgIntense,
                                       const std::vector<color> &regular,
                                       const std::vector<color> &intense) const {

    // Creates a backup config file
    if (!fs::exists(this->absolutePath("_BACKUP"))) {
        fs::copy_file(this->absolutePath(""), this->absolutePath("_BACKUP"));
    }

    std::ifstream fIn{absolutePath("")};
    std::ofstream fOut{absolutePath("_out")};

    if (!fIn || !fOut) {
        throw TermColorException{"Errors opening files"};
    }

    // Read input file by line and replace each of it lines
    std::string tempStr;
    int cIterNo = 0;
    const auto bgColor{"bgcolor="};
    const auto fgColor{"fgcolor="};
    const auto cPresent{"color_preset="};
    const auto cPallete{"palette_color_"}; // palette_color_0=rgb(0,0,0)

    while (std::getline(fIn, tempStr)) {
        // background
        if (tempStr.find(bgColor) != std::string::npos) {
            fOut << bgColor << "rgb(" << bgfg.at(0).getCommaSeparatedStr() << ")\n";
        }
        // foreground
        else if (tempStr.find(fgColor) != std::string::npos) {
            fOut << fgColor << "rgb(" << bgfg.at(1).getCommaSeparatedStr() << ")\n";
        }
        // colors
        else if (tempStr.find(cPallete) != std::string::npos) {
            // regular colors
            if (cIterNo < regular.size()) {
                fOut << cPallete
                     << std::to_string(cIterNo) << "=rgb("
                     << regular.at(cIterNo).getCommaSeparatedStr()
                     << ")\n";
            }
            // intense colors
            else {
                fOut << cPallete
                     << std::to_string(cIterNo) << "=rgb("
                     << intense.at(cIterNo - regular.size()).getCommaSeparatedStr()
                     << ")\n";
            }
            ++cIterNo;
        }
        // name of color-scheme
        else if (tempStr.find(cPresent) != std::string::npos) {
            fOut << cPresent << "TermColor\n";
        }
        else {
            fOut << tempStr << "\n";
        }
    }

    fIn.close();
    fOut.close();

    // Overwrite file
    fs::rename(this->absolutePath("_out"), this->absolutePath(""));
    fs::remove(this->absolutePath("_out"));
}

}
