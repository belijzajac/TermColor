#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "../Writer.h"

class JsonWriter : public Writer {
public:
    explicit JsonWriter();
    ~JsonWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &) const override;
};

#endif // JSONWRITER_H
