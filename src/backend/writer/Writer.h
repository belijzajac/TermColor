#ifndef ABSTRACTWRITER_H
#define ABSTRACTWRITER_H

#include <backend/dominantcolor/DominantColor.h>
#include <zconf.h>
#include <pwd.h>

struct filename {
    std::string prefix; // termcolor_
    std::string format; // .colorscheme
};

class Writer {
public:
    virtual ~Writer() = default;

    // Get username of current user logged in on Linux
    static const std::string getUsername() {
        uid_t uid { geteuid() };
        struct passwd *pw { getpwuid(uid) };
        if(pw)
            return std::string(pw->pw_name);
        return {};
    }

    // Writes color-scheme to file
    virtual void writeToLocation(const std::string &name,
                                 const std::vector<color> &,
                                 const std::vector<color> &) const = 0;

    // Returns an absolute location to color schemes save folder
    const std::string absolutePath(const std::string &fileName) const {
        return "/home/" + getUsername() + "/" + location_ + fileInfo_.prefix + fileName + fileInfo_.format;
    }

protected:
    Writer(const std::string &loc, const filename &fileInfo)
            : location_{loc}, fileInfo_{fileInfo} {}

    std::string location_;
    filename fileInfo_;
};

#endif // ABSTRACTWRITER_H
