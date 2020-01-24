#include "JsonWriter.h"

// rapidjson
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <backend/exception/Exception.h>
#include <algorithm>
#include <fstream>

namespace TermColor {

using namespace TermColor::Utils;
using namespace rapidjson;

JsonWriter::JsonWriter()
    : Writer("", {"termcolor_", ".json"})
{

}

// A helper function for writing comma separated color values
void writeCommaSepStr(PrettyWriter<StringBuffer> &writer, const char *blockName, const std::vector<color> &cl) {
    writer.String(blockName);
    writer.StartArray();

    std::for_each(cl.begin(), cl.end(), [&writer](const color &c){
        const char *colorStr = c.getCommaSeparatedStr().c_str();
        writer.String(colorStr);
    });

    writer.EndArray();
}

// A helper function for writing colors in their hexadecimal format
void writeHexStr(PrettyWriter<StringBuffer> &writer, const char *blockName, const std::vector<color> &cl) {
    writer.String(blockName);
    writer.StartArray();

    std::for_each(cl.begin(), cl.end(), [&writer](const color &c){
        const char *colorStr = c.getHexStr().c_str();
        writer.String(colorStr);
    });

    writer.EndArray();
}

void JsonWriter::writeToLocation(const std::string &name,
                                 const std::vector<color> &bgfg,
                                 const std::vector<color> &bgfgIntense,
                                 const std::vector<color> &regular,
                                 const std::vector<color> &intense) const {

    std::ofstream f{name};
    if (!f)
        throw Exception{"bad file name: " + name};

    // Prepare buffer and document
    StringBuffer stringBuff;
    Document doc;

    // Construct writer
    PrettyWriter<StringBuffer> writer{stringBuff};

    // Start writing json
    writer.StartObject();

    // RGB colors
    writer.String("RGB");
    writer.StartObject();
    writeCommaSepStr(writer, "BG/FG regular colors", bgfg);
    writeCommaSepStr(writer, "BG/FG intense colors", bgfgIntense);
    writeCommaSepStr(writer, "Regular colors", regular);
    writeCommaSepStr(writer, "Intense colors", intense);
    writer.EndObject();

    // HEX colors
    writer.String("HEX");
    writer.StartObject();
    writeHexStr(writer, "BG/FG regular colors", bgfg);
    writeHexStr(writer, "BG/FG intense colors", bgfgIntense);
    writeHexStr(writer, "Regular colors", regular);
    writeHexStr(writer, "Intense colors", intense);
    writer.EndObject();

    // End writing json
    writer.EndObject();

    // Write just generated json bufer to file
    f << stringBuff.GetString();
}

}
