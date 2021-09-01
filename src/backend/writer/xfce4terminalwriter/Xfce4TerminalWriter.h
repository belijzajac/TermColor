#ifndef XFCE4TERMINALWRITER_H
#define XFCE4TERMINALWRITER_H

#include "backend/writer/Writer.h"

namespace TermColor {

class Xfce4TerminalWriter : public Writer {
public:
    explicit Xfce4TerminalWriter();
    ~Xfce4TerminalWriter() = default;

    void writeToLocation(std::string_view name,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &,
                         const std::vector<color_t> &) const override;
};

}

#endif // XFCE4TERMINALWRITER_H
