#ifndef LXTERMINALWRITER_H
#define LXTERMINALWRITER_H

#include "../Writer.h"

class LXTerminalWriter : public Writer {
public:
    explicit LXTerminalWriter();
    ~LXTerminalWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &) const override;
};

#endif // LXTERMINALWRITER_H
