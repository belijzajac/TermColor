#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "backend/writer/Writer.h"

namespace TermColor {

class JsonWriter : public Writer {
public:
    explicit JsonWriter();
    ~JsonWriter() = default;

    void writeToLocation(std::string_view name,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &) override;
};

}

#endif // JSONWRITER_H
