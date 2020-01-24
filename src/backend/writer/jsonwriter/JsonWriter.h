#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "../Writer.h"

namespace TermColor {

class JsonWriter : public Writer {
public:
    explicit JsonWriter();
    ~JsonWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &) const override;
};

}

#endif // JSONWRITER_H
