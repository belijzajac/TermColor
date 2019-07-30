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

void KonsoleWriter::writeToLocation(const std::string &name, const std::vector<color> &colors) const {
    const std::string pathToFile{absolutePath(name)};
    std::ofstream f{pathToFile};

    if (!f)
        throw Exception{"bad file name: " + pathToFile};

    // Write dominant colors
    int colCount = 0;
    std::for_each(colors.begin(), colors.end(), [this, &f, &colCount](const color &color) {
        f << "[" + nameColors_.at(colCount) + "]\n";
        f << "Color=" << color.r << "," << color.g << "," << color.b << "\n\n";
        ++colCount;
    });

    f.close();
}