#ifndef NODE_SKIA_COLOR_H
#define NODE_SKIA_COLOR_H

#include <string>
#include <unordered_map>
#include <include/core/SkColor.h>

using std::string;
using std::unordered_map;
using SkColorMap = unordered_map<string, SkColor>;

namespace node_skia_color {

    class SkColorParser {
    private:
        static const SkColorMap semantic_color_map;
    public:
        static SkColor4f rgba_from_string(string &str);
    };
}

#endif
