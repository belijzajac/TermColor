#ifndef KONSOLEWRITER_H
#define KONSOLEWRITER_H

#include "../Writer.h"

namespace TermColor {

class KonsoleWriter : public Writer {
public:
    explicit KonsoleWriter();
    ~KonsoleWriter() = default;

    void writeToLocation(std::string_view name,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &) const override;

private:
    std::vector<std::string> nameColors_;
};

}

#endif // KONSOLEWRITER_H
