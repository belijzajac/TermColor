#include "Writer.h"
#include <zconf.h>
#include <pwd.h>
#include <filesystem>

Writer::Writer(const std::string &loc, const filename &fileInfo)
    : location_{loc}
    , fileInfo_{fileInfo}
{
    checkDirectory("/home/" + getUsername() + "/" + location_);
}

const std::string Writer::getUsername() {
    uid_t uid{geteuid()};
    struct passwd *pw{getpwuid(uid)};
    if (pw)
        return std::string(pw->pw_name);
    return {};
}

const std::string Writer::absolutePath(const std::string &fileName) const {
    return "/home/" + getUsername() + "/" + location_ + fileInfo_.prefix + fileName + fileInfo_.format;
}

void Writer::checkDirectory(const std::string &path) {
    std::filesystem::path dirPath{path};
    std::filesystem::directory_entry dirEntry{dirPath};

    // Create directory if it doesn't exist
    if (!dirEntry.exists())
        std::filesystem::create_directories(dirPath);
}
