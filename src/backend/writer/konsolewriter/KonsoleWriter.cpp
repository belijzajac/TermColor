#include "KonsoleWriter.h"
#include <backend/exception/Exception.h>

#include <algorithm>
#include <fstream>

KonsoleWriter::KonsoleWriter() : Writer(
        {".local/share/konsole/"},
        {{"termcolor_"}, {".colorscheme"}}) {
    // Fill nameColors_ with ColorX, where 0 <= X <= 7
    for (int i = 0; i <= 7; ++i)
        nameColors_.push_back("Color" + std::to_string(i));
}

void KonsoleWriter::writeToLocation(const std::string &name,
                                    const std::vector<color> &domColors,
                                    const std::vector<color> &intColors) const {
    const std::string pathToFile{ absolutePath(name) };
    std::ofstream f{pathToFile};

    if (!f)
        throw Exception{"bad file name: " + pathToFile};

    // Write dominant colors
    std::for_each(domColors.begin(), domColors.end(), [=, &f, count = 0](const color &color) mutable {
        f << "[" + nameColors_.at(count) + "]\n";
        f << "Color=" << color.r << "," << color.g << "," << color.b << "\n\n";
        ++count;
    });

    // Write intense colors
    std::for_each(intColors.begin(), intColors.end(), [=, &f, count = 0](const color &color) mutable {
        f << "[" + nameColors_.at(count) + "Intense" + "]\n";
        f << "Color=" << color.r << "," << color.g << "," << color.b << "\n\n";
        ++count;
    });

    f.close();
}
