#include "Writer.h"

#include "backend/exception/Exception.h"
#include <zconf.h>
#include <pwd.h>
#include <filesystem>

namespace TermColor {

Writer::Writer(std::string_view loc, const filename &fileInfo)
    : location_{loc.data()}
    , fileInfo_{fileInfo}
{
    checkDirectory("/home/" + getUsername() + "/" + location_);
}

const std::string Writer::getUsername() {
    using namespace TermColor::Utils;
    uid_t uid{geteuid()};

    if (struct passwd *pw{getpwuid(uid)}; pw) {
        return std::string(pw->pw_name);
    }

    throw TermColorException{"cannot get current logged in user's username"};
}

const std::string Writer::absolutePath(std::string_view fileName) const {
    return "/home/" + getUsername() + "/" + location_ + fileInfo_.prefix + fileName.data() + fileInfo_.format;
}

void Writer::checkDirectory(std::string_view path) {
    std::filesystem::path dirPath{path.data()};
    std::filesystem::directory_entry dirEntry{dirPath};

    // Create directory if it doesn't exist
    if (!dirEntry.exists()) {
        std::filesystem::create_directories(dirPath);
    }
}

}
