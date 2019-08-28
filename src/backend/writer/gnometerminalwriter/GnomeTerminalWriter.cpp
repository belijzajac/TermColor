#include "GnomeTerminalWriter.h"
#include <backend/exception/Exception.h>
#include <boost/process.hpp>

using namespace boost::process;

GnomeTerminalWriter::GnomeTerminalWriter()
    : Writer("", {"termcolor_", ""}) {

}

void GnomeTerminalWriter::writeToLocation(const std::string &name,
                                          const std::vector<color> &bgfg,
                                          const std::vector<color> &bgfgIntense,
                                          const std::vector<color> &regular,
                                          const std::vector<color> &intense) const {

    // ...
}
