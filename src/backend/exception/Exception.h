#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <bits/exception.h>

namespace TermColor::Utils {

class TermColorException : public std::exception {
public:
    explicit TermColorException(std::string_view msg) : msg_{msg} {}
    const char *what() const noexcept override { return msg_.data(); }

private:
    std::string_view msg_;
};

}

#endif // EXCEPTION_H
