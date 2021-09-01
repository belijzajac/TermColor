#include "JsonWriter.h"

// rapidjson
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
//..
#include "backend/exception/Exception.h"
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
void writeCommaSepStr(PrettyWriter<StringBuffer> &writer, std::string_view blockName, const std::vector<color_t> &cl) {
    writer.String(blockName.data());
    writer.StartArray();

    std::for_each(cl.begin(), cl.end(), [&writer](const color_t &c){
        const char *colorStr = c.getCommaSeparatedStr().c_str();
        writer.String(colorStr);
    });

    writer.EndArray();
}

// A helper function for writing colors in their hexadecimal format
void writeHexStr(PrettyWriter<StringBuffer> &writer, std::string_view blockName, const std::vector<color_t> &cl) {
    writer.String(blockName.data());
    writer.StartArray();

    std::for_each(cl.begin(), cl.end(), [&writer](const color_t &c){
        const char *colorStr = c.getHexStr().c_str();
        writer.String(colorStr);
    });

    writer.EndArray();
}

void JsonWriter::writeToLocation(std::string_view name,
                                 const std::vector<color_t> &bgfg,
                                 const std::vector<color_t> &bgfgIntense,
                                 const std::vector<color_t> &regular,
                                 const std::vector<color_t> &intense) const {

    if (std::ofstream f{name.data()}; f) {
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
    } else {
        throw TermColorException{"bad file name: " + std::string(name.data())};
    }
}

}
