#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "backend/writer/Writer.h"

namespace TermColor {

class JsonWriter : public Writer {
public:
    explicit JsonWriter();
    ~JsonWriter() = default;

    void writeToLocation(std::string_view name,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &) const override;
};

}

#endif // JSONWRITER_H
