#include "KonsoleWriter.h"

#include "backend/exception/Exception.h"
#include <algorithm>
#include <fstream>

namespace TermColor {

using namespace TermColor::Utils;

KonsoleWriter::KonsoleWriter()
    : Writer(".local/share/konsole/"
    , {{"termcolor_"}, {".colorscheme"}})
{
    // Fill nameColors_ with ColorX, where 0 <= X <= 7
    for (int i = 0; i <= 7; ++i) {
        nameColors_.push_back("Color" + std::to_string(i));
    }
}

void KonsoleWriter::writeToLocation(std::string_view name,
                                    const std::vector<color_t> &bgfg,
                                    const std::vector<color_t> &bgfgIntense,
                                    const std::vector<color_t> &regular,
                                    const std::vector<color_t> &intense) {

    const std::string pathToFile{ absolutePath(name.data()) };

    if (std::ofstream f{pathToFile}; f) {
        // Write general info
        f << "[General]\n";
        f << "Description=TermColor " << name << "\n";
        f << "Wallpaper=\n\n";

        // Write background and foreground info
        const std::vector<std::string> bgfgTextProp {"Background", "Foreground"};
        std::for_each(bgfg.begin(), bgfg.end(), [=, &f, count = 0](const color_t &color) mutable {
            f << "[" << bgfgTextProp.at(count) << "]\n";
            f << "Color=" << color.getCommaSeparatedStr() << "\n\n";
            ++count;
        });

        // Write background and foreground info of intense colors
        const std::vector<std::string> bgfgIntTextProp {"BackgroundIntense", "ForegroundIntense"};
        std::for_each(bgfgIntense.begin(), bgfgIntense.end(), [=, &f, count = 0](const color_t &color) mutable {
            f << "[" << bgfgIntTextProp.at(count) << "]\n";
            f << "Color=" << color.getCommaSeparatedStr() << "\n\n";
            ++count;
        });

        // Write dominant colors
        std::for_each(regular.begin(), regular.end(), [=, &f, count = 0](const color_t &color) mutable {
            f << "[" << nameColors_.at(count) << "]\n";
            f << "Color=" << color.getCommaSeparatedStr() << "\n\n";
            ++count;
        });

        // Write intense colors
        std::for_each(intense.begin(), intense.end(), [=, &f, count = 0](const color_t &color) mutable {
            f << "[" << nameColors_.at(count) << "Intense" << "]\n";
            f << "Color=" << color.getCommaSeparatedStr() << "\n\n";
            ++count;
        });
    } else {
        throw TermColorException{"bad file name: " + pathToFile};
    }
}

}
