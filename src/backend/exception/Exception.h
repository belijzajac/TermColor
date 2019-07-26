#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <bits/exception.h>
#include <string>

class Exception : public std::exception {
public:
    explicit Exception(const std::string &msg) : msg_{msg} {}
    const char *what() const noexcept override { return msg_.c_str(); }

private:
    std::string msg_;
};

#endif // EXCEPTION_H
