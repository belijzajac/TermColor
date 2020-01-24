#ifndef XFCE4TERMINALWRITER_H
#define XFCE4TERMINALWRITER_H

#include "../Writer.h"

namespace TermColor {

class Xfce4TerminalWriter : public Writer {
public:
    explicit Xfce4TerminalWriter();
    ~Xfce4TerminalWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &,
                         const std::vector<TermColor::Utils::color> &) const override;
};

}

#endif // XFCE4TERMINALWRITER_H
