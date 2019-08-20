#ifndef KONSOLEWRITER_H
#define KONSOLEWRITER_H

#include "../Writer.h"

class KonsoleWriter : public Writer {
public:
    explicit KonsoleWriter();
    ~KonsoleWriter() = default;

    void writeToLocation(const std::string &name,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &,
                         const std::vector<color> &) const override;

private:
    std::vector<std::string> nameColors_;
};

#endif // KONSOLEWRITER_H
