#include "Xfce4TerminalWriter.h"

#include <backend/exception/Exception.h>
#include <algorithm>
#include <fstream>

namespace TermColor {

using namespace TermColor::Utils;

Xfce4TerminalWriter::Xfce4TerminalWriter()
    : Writer(".local/share/xfce4/terminal/colorschemes/"
    , {"termcolor_", ".theme"})
{

}

void Xfce4TerminalWriter::writeToLocation(std::string_view name,
                                          const std::vector<color> &bgfg,
                                          const std::vector<color> &bgfgIntense,
                                          const std::vector<color> &regular,
                                          const std::vector<color> &intense) const {

    const std::string pathToFile{ absolutePath(name.data()) };

    if (std::ofstream f{pathToFile}; f) {
        // Write general info
        f << "[Scheme]\n";
        f << "Name=" << this->fileInfo_.prefix << name.data() << "\n";

        // Write background and foreground
        const std::vector<std::string> bgfgStr {"ColorBackground=", "ColorForeground="};
        std::for_each(bgfg.begin(), bgfg.end(), [=, &f, count = 0](const color &color) mutable {
            f << bgfgStr.at(count) << "rgb(" << color.getCommaSeparatedStr() << ")\n";
            ++count;
        });

        // Start writing colors
        f << "ColorPalette=";

        // Regular colors
        std::for_each(regular.begin(), regular.end(), [=, &f](const color &color) {
            f << "rgb(" << color.getCommaSeparatedStr() << ");";
        });

        // Intense colors
        std::for_each(intense.begin(), intense.end(), [=, &f](const color &color) {
            f << "rgb(" << color.getCommaSeparatedStr() << ");";
        });
    } else {
        throw TermColorException{"bad file name: " + pathToFile};
    }
}

}
