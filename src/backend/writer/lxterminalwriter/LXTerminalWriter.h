#ifndef LXTERMINALWRITER_H
#define LXTERMINALWRITER_H

#include "../Writer.h"

namespace TermColor {

class LXTerminalWriter : public Writer {
public:
    explicit LXTerminalWriter();
    ~LXTerminalWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &) const override;
};

}

#endif // LXTERMINALWRITER_H
