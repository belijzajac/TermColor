#ifndef KONSOLEWRITER_H
#define KONSOLEWRITER_H

#include "backend/writer/Writer.h"

namespace TermColor {

class KonsoleWriter : public Writer {
public:
    explicit KonsoleWriter();
    ~KonsoleWriter() = default;

    void writeToLocation(std::string_view name,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &) override;

private:
    std::vector<std::string> nameColors_;
};

}

#endif // KONSOLEWRITER_H
