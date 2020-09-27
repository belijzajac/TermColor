#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <backend/dominantcolor/DominantColor.h>

namespace TermColor {

class Writer {
public:
    virtual ~Writer() = default;

    // Get username of current user logged in on Linux
    static const std::string getUsername();

    // Writes color-scheme to file
    virtual void writeToLocation(std::string_view name,
                                 const std::vector<TermColor::Utils::color> &,              // BG/FG colors
                                 const std::vector<TermColor::Utils::color> &,              // Intense BG/FG colors
                                 const std::vector<TermColor::Utils::color> &,              // Regular colors
                                 const std::vector<TermColor::Utils::color> &) const = 0;   // Intense colors

    // Returns an absolute location to color schemes save folder
    const std::string absolutePath(std::string_view fileName) const;

private:
    // Checks if provided directory exists, if not - creates it
    void checkDirectory(std::string_view path);

    // Holds color-scheme file information
    struct filename {
        std::string prefix; // e.g. "termcolor_"
        std::string format; // e.g. for Konsole it's ".colorscheme"
    };

protected:
    Writer(std::string_view loc, const filename &fileInfo);

    std::string location_;
    filename fileInfo_;
};

}

#endif // WRITER_H
