#ifndef  GNOMETERMINALWRITER_H
#define  GNOMETERMINALWRITER_H

#include "../Writer.h"

class GnomeTerminalWriter : public Writer {
public:
    explicit GnomeTerminalWriter();
    ~GnomeTerminalWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &) const override;
};

#endif // GNOMETERMINALWRITER_H
